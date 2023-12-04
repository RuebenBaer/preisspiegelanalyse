#include <fstream>
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <stdlib.h>
#include "lvSruct.h"
#include "stack.h"

void einlesen(std::ifstream &file, std::vector<Position*> &LV, std::vector<Bieter*> lstBieter);
void auslesen(std::vector<Position*> &LV);
void Aufraeumen(std::vector<Position*> LV, std::vector<Bieter*> lstBieter);
void ErsteZeileLesen(std::string, std::vector<Bieter*>&);

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
	std::cout<<"PositionNr. | Kurztext | Langtext | mitGP | Menge | Einheit | Schwerpunktpos. | Bieter 1 | Bieter 2 | Bieter 3 | usw.\n\n";
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
	auslesen(LV);
	
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
	if(str.empty())
		pos.evtlPos = false;
	else
		pos.evtlPos = true;
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

void Lese_schwerPunkt(std::string str, Position &pos)
{
	if(str.empty())
		pos.schwerPunkt = false;
	else
		pos.schwerPunkt = true;
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

void einlesen(std::ifstream &file, std::vector<Position*> &LV, std::vector<Bieter*> lstBieter)
{
	std::streampos fileAnfang = file.tellg();
	std::string zeile;

	std::getline(file, zeile);
	ErsteZeileLesen(zeile, lstBieter);

	std::cout<<"erste Zeile eingelesen\n"<<lstBieter.size()<<" Bieter eingelesen:\n";
	for(long long unsigned int i = 0; i < lstBieter.size(); i++)
	{
		std::cout<<"Bieter "<<i<<": "<<lstBieter[i]->name<<"\n";
	}

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
					Lese_schwerPunkt(str, *position);
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

void Aufraeumen(std::vector<Position*> LV, std::vector<Bieter*> lstBieter)
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

void auslesen(std::vector<Position*> &LV)
{
	std::cout<<"Entered Auslesen\n";
	for(long long unsigned int i = 0; i < LV.size(); i++)
	{
		std::cout<<LV[i]->posNr<<" | "<<LV[i]->kurzText<<" | "<<LV[i]->langText<<" | "<<LV[i]->evtlPos<<" | ";
		std::cout<<LV[i]->menge<<" | "<<LV[i]->einheit<<" | "<<LV[i]->schwerPunkt;
		for(int j = 0; j < LV[i]->anzAngebote; j++)
		{
			std::cout<<" | "<<LV[i]->lstAngebote[j].EP;
		}
		std::cout<<"\n";
	}
	return;
}