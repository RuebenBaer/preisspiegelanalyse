#include <fstream>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <cstring>
#include <string>
#include <vector>
#include <stdlib.h>
#include "lvSruct.h"
#include "stack.h"

void einlesen(std::ifstream &file, std::vector<Position*> &LV, std::vector<Bieter*> &lstBieter);
void ErsteZeileLesen(std::string, std::vector<Bieter*>&);
void auslesen(std::vector<Position*> &LV, std::vector<Bieter*> &lstBieter);
void Aufraeumen(std::vector<Position*> &LV, std::vector<Bieter*> &lstBieter);
void LvAnalyse(std::vector<Position*> &LV, std::vector<Bieter*> &lstBieter);

double ABWEICHUNG = 0.2, HOHE_ABWEICHUNG = 0.5, EXTREME_ABWEICHUNG = 1.0;

void KommaGegenPunkt(std::string &str)
{
	for(long long unsigned int i = 0; i < str.length(); i++)
	{
		if(str[i] == ',')str[i] = '.';
	}
	return;
}

void printHelp()
{
	std::cout<<"Erforderliche Reihenfolge der Einträge:\n";
	std::cout<<"PositionNr. | Kurztext | Langtext | mitGP | Menge | Einheit | Kostenrisiko | Bieter 1 | Bieter 2 | Bieter 3 | usw.\n\n";
	std::cout<<"In der ersten Zeile der Datei sind die Namen der Bieter zu hinterlegen\n\n";
	return;
}

int main(int argc, char** argv)
{
	if(argc < 2)
	{
		std::cout<<"Verwendung:\n"<<argv[0]<<" [Datei]\n";
		system("PAUSE");
		return EXIT_FAILURE;
	}
	
	if(strcmp(argv[1], "--help") == 0)
	{
		printHelp();
		return EXIT_SUCCESS;
	}
	
	std::ifstream file;
	file.open(argv[1], std::ios::in);
	if(!file.good())
	{
		std::cout<<"Fehler beim oeffnen der Datei "<<argv[1]<<"\n";
		system("PAUSE");		
		return EXIT_FAILURE;
	}

	std::vector<Position*> LV;
	std::vector<Bieter*> lstBieter;
	
	std::streampos fileAnfang = file.tellg();
	
	einlesen(file, LV, lstBieter);
	
	LvAnalyse(LV, lstBieter);
	auslesen(LV, lstBieter);
	
	file.close();
	Aufraeumen(LV, lstBieter);
	system("PAUSE");
	return EXIT_SUCCESS;
}

void Lese_posNr(std::string str, Position &pos)
{
	pos.posNr = str;
	return;
}

void Lese_kurzText(std::string str, Position &pos)
{
	pos.kurzText = str;
	return;
}

void Lese_langText(std::string str, Position &pos)
{
	pos.langText = str;
	return;
}

void Lese_evtlPos(std::string str, Position &pos)
{
	if(str.empty())return;
	char ersterBuchstabe = str.at(0);
	std::cout<<ersterBuchstabe<<"\n";
	pos.evtlPos = ersterBuchstabe;
	return;
}

void Lese_menge(std::string str, Position &pos)
{
	KommaGegenPunkt(str);
	pos.menge = atof(str.c_str());
	return;
}

void Lese_einheit(std::string str, Position &pos)
{
	pos.einheit = str;
	return;
}

void Lese_risiko(std::string str, Position &pos)
{
	if(str.empty())
		pos.risiko = 0;
	else
		pos.risiko = atoi(str.c_str())%3;
	return;
}

void Lese_Bieter(std::string str, Position &pos, long long unsigned int &i)
{
	KommaGegenPunkt(str);
	pos.lstAngebote[i].EP = atof(str.c_str());
	pos.lstAngebote[i].bieterNr = i;
	return;
}

void ErsteZeileLesen(std::string zeile, std::vector<Bieter*> &bieter)
{
	size_t pos = 0;
	size_t neuPos = 0;
	int stelle = 0;
	while(1)
	{
		neuPos = zeile.find(";", pos);
		if(neuPos == std::string::npos)
		{
			break;
		}
		if(stelle > 6)
		{
			Bieter* neuBieter = new Bieter;
			neuBieter->name = zeile.substr(pos, neuPos-pos);
			bieter.push_back(neuBieter);
		}
		stelle++;
		pos = neuPos + 1;
	}
	return;
}

void einlesen(std::ifstream &file, std::vector<Position*> &LV, std::vector<Bieter*> &lstBieter)
{
	std::streampos fileAnfang = file.tellg();
	std::string zeile;

	std::getline(file, zeile);
	ErsteZeileLesen(zeile, lstBieter);

	while(!file.eof())
	{
		std::getline(file, zeile);

		int aktFkt = 0;
		long long unsigned int bieterNr = 0;
		size_t pos = 0;
		size_t neuPos = 0;
		
		Position* position = new Position();
		position->lstAngebote = new Angebot[lstBieter.size()];
		position->anzAngebote = lstBieter.size();
		LV.push_back(position);
		
		do{
			neuPos = zeile.find(";", pos);
			if(neuPos == std::string::npos)
			{
				break;
			}
			std::string str = zeile.substr(pos, neuPos-pos);
			switch (aktFkt){
				case 0:
					Lese_posNr(str, *position);
					aktFkt++;
					break;
				case 1:
					Lese_kurzText(str, *position);
					aktFkt++;
					break;
				case 2:
					Lese_langText(str, *position);
					aktFkt++;
					break;
				case 3:
					Lese_evtlPos(str, *position);
					aktFkt++;
					break;
				case 4:
					Lese_menge(str, *position);
					aktFkt++;
					break;
				case 5:
					Lese_einheit(str, *position);
					aktFkt++;
					break;
				case 6:
					Lese_risiko(str, *position);
					aktFkt++;
					break;
				case 7:
					Lese_Bieter(str, *position, bieterNr);
					bieterNr++;
					break;
			}
			pos = neuPos + 1;
		}while(bieterNr < lstBieter.size());
	};
	return;
}

void Aufraeumen(std::vector<Position*> &LV, std::vector<Bieter*> &lstBieter)
{
	for(std::vector<Bieter*>::iterator it = lstBieter.begin(); it != lstBieter.end(); it++)
	{
		delete *it;
	}
	for(std::vector<Position*>::iterator it = LV.begin(); it != LV.end(); it++)
	{
		delete *it;
	}
	std::cout<<"Aufraeumen beendet\n";
	return;
}

void auslesen(std::vector<Position*> &LV, std::vector<Bieter*> &lstBieter)
{
	char buffer[16];
	std::ofstream ausgabe;
	ausgabe.open("Analyse.txt", std::ios::out | std::ios::trunc);
	if(ausgabe.good())
	{
		for(long long unsigned int itB = 0; itB < lstBieter.size(); itB++)
		{
			ausgabe<<"Analyse "<<lstBieter[itB]->name<<":\n";
			ausgabe<<std::setw(16)<<"Position Nr."<<" | "<<std::setw(32)<<std::setiosflags(std::ios::left);
			ausgabe<<"Kurztext"<<std::resetiosflags(std::ios::left)<<" | ";
			ausgabe<<std::setw(10)<<"Menge"<<std::setw(5)<<"Dim"<<" | ";
			ausgabe<<std::setw(10)<<"EP geboten"<<" | "<<std::setw(10)<<"Median"<<" | Analyseergebnis";
			ausgabe<<"\n";
			for(long long unsigned int i = 0; i < LV.size(); i++)
			{
				if(LV[i]->lstAngebote[itB].analyse.empty())continue;
				ausgabe<<std::setw(16)<<LV[i]->posNr<<" | "<<std::setw(32)<<std::setiosflags(std::ios::left);
				ausgabe<<LV[i]->kurzText.substr(0, 31)<<std::resetiosflags(std::ios::left)<<" | ";
				sprintf(buffer, "%.2f", LV[i]->menge);
				ausgabe<<std::setw(10)<<buffer<<std::setw(5)<<LV[i]->einheit<<" | ";
				sprintf(buffer, "%.2f", LV[i]->lstAngebote[itB].EP);
				ausgabe<<std::setw(10)<<buffer<<" | ";
				sprintf(buffer, "%.2f", LV[i]->median);
				ausgabe<<std::setw(10)<<buffer<<" | "<<LV[i]->lstAngebote[itB].analyse;
				ausgabe<<"\n";
			}
			ausgabe<<"\n";
		}
	}
	ausgabe.close();
	ausgabe.open("Analyse.csv", std::ios::out | std::ios::trunc);
	if(ausgabe.good())
	{
		for(long long unsigned int itB = 0; itB < lstBieter.size(); itB++)
		{
			ausgabe<<"Analyse "<<lstBieter[itB]->name<<":\n";
			ausgabe<<"Position Nr.;Kurztext;Menge;Dim;EP geboten;Median;Analyseergebnis\n";
			for(long long unsigned int i = 0; i < LV.size(); i++)
			{
				if(LV[i]->lstAngebote[itB].analyse.empty())continue;
				ausgabe<<LV[i]->posNr<<";"<<LV[i]->kurzText<<";";
				sprintf(buffer, "%.2f", LV[i]->menge);
				ausgabe<<buffer<<";"<<LV[i]->einheit<<";";
				sprintf(buffer, "%.2f", LV[i]->lstAngebote[itB].EP);
				ausgabe<<buffer<<";";
				sprintf(buffer, "%.2f", LV[i]->median);
				ausgabe<<buffer<<";"<<LV[i]->lstAngebote[itB].analyse;
				ausgabe<<"\n";
			}
			ausgabe<<"\n";
		}
	}
	ausgabe.close();
	for(long long unsigned int itB = 0; itB < lstBieter.size(); itB++)
	{
		std::cout<<"Analyse "<<lstBieter[itB]->name<<":\n";
		std::cout<<std::setw(16)<<"Position Nr."<<" | "<<std::setw(32)<<std::setiosflags(std::ios::left);
		std::cout<<"Kurztext"<<std::resetiosflags(std::ios::left)<<" | ";
		std::cout<<std::setw(10)<<"Menge"<<std::setw(5)<<"Dim"<<" | ";
		std::cout<<std::setw(10)<<"EP geboten"<<" | "<<std::setw(10)<<"Median"<<" | Analyseergebnis";
		std::cout<<"\n";
		for(long long unsigned int i = 0; i < LV.size(); i++)
		{
			if(LV[i]->lstAngebote[itB].analyse.empty())continue;
			std::cout<<std::setw(16)<<LV[i]->posNr<<" | "<<std::setw(32)<<std::setiosflags(std::ios::left);
			std::cout<<LV[i]->kurzText.substr(0, 31)<<std::resetiosflags(std::ios::left)<<" | ";
			sprintf(buffer, "%.2f", LV[i]->menge);
			std::cout<<std::setw(10)<<buffer<<std::setw(5)<<LV[i]->einheit<<" | ";
			sprintf(buffer, "%.2f", LV[i]->lstAngebote[itB].EP);
			std::cout<<std::setw(10)<<buffer<<" | ";
			sprintf(buffer, "%.2f", LV[i]->median);
			std::cout<<std::setw(10)<<buffer<<" | "<<LV[i]->lstAngebote[itB].analyse;
			std::cout<<"\n";
		}
		std::cout<<"\n";
	}
	return;
}

void LvAnalyse(std::vector<Position*> &LV, std::vector<Bieter*> &lstBieter)
{
	stack stapel;
	double median, abweichung;
	char buffer[32];
	
	for(std::vector<Position*>::iterator it = LV.begin(); it != LV.end(); it++)
	{
		for(int i = 0; i < (*it)->anzAngebote; i++)
		{
			stapel.push((*it)->lstAngebote[i].EP);
		}
		median = stapel.median();
		if(median == 0)continue;
		(*it)->median = median;
		for(int i = 0; i < (*it)->anzAngebote; i++)
		{
			abweichung = ((*it)->lstAngebote[i].EP / median)-1;
			if(abs(abweichung) > ABWEICHUNG)
			{
				sprintf(buffer, "%0.1f", abweichung*100);
				(*it)->lstAngebote[i].analyse += buffer;
				(*it)->lstAngebote[i].analyse += " %: ";
			}
			if(abs(abweichung) > EXTREME_ABWEICHUNG)
			{
				(*it)->lstAngebote[i].analyse += "extrem ";
				
			}
			else if(abs(abweichung) > HOHE_ABWEICHUNG)
			{
				(*it)->lstAngebote[i].analyse += "sehr ";
			}
			else if(abs(abweichung) <= ABWEICHUNG)
			{
				continue;
			}
			
			if(abweichung > 0)
			{
				(*it)->lstAngebote[i].analyse += "hoher EP";
			}
			else
			{
				(*it)->lstAngebote[i].analyse += "niedriger EP";
			}

			if(((*it)->evtlPos == 'E') || ((*it)->evtlPos == 'P'))
			{
				if((*it)->evtlPos == 'E')
					(*it)->lstAngebote[i].analyse += ", Evtl.-Pos.: ";
				else
					(*it)->lstAngebote[i].analyse += ", Preisabfrage: ";
				switch((*it)->risiko)
				{
					case 0:
						(*it)->lstAngebote[i].analyse += "moderates Kostenpotential";
						break;
					case 1:
						(*it)->lstAngebote[i].analyse += "hohes Kostenpotential";
						break;
					case 2:
						(*it)->lstAngebote[i].analyse += "sehr hohes Kostenpotential";
						break;
					default:
						break;
				}
			}
		}
	}
	return;
}