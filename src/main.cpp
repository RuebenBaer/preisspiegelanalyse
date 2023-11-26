#include <fstream>
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <stdlib.h>
#include "lvSruct.h"

void einlesen(std::ifstream &file, std::vector<Position*> &LV);
void ErsteZeileLesen(std::string, std::vector<Bieter*>&);

void printHelp()
{
	std::cout<<"Erforderliche Reihenfolge der Einträge:\t";
	std::cout<<"PositionNr.\tKurztext\tLangtext\tmitGP\tMenge\tEinheit\tSchwerpunktpos.\tBieter 1\tBieter 2;Bieter 3; usw.\n";
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
	
	std::streampos fileAnfang = file.tellg();
	
	einlesen(file, LV);
	
	file.close();
	system("PAUSE");
	return EXIT_SUCCESS;
}

void einlesen(std::ifstream &file, std::vector<Position*> &LV)
{
	std::streampos fileAnfang = file.tellg();
	std::string zeile;
	
	std::vector<Bieter*> lstBieter;
	std::getline(file, zeile);
	ErsteZeileLesen(zeile, lstBieter);

	std::cout<<"erste Zeile eingelesen\n"<<lstBieter.size()<<" Bieter eingelesen:\n";
	for(long long unsigned int i = 0; i < lstBieter.size(); i++)
	{
		std::cout<<lstBieter[i]->name<<"\n";
	}
	
	Position tempPos;
	while(!file.eof())
	{
		std::getline(file, zeile);
		std::cout<<"Verarbeite Zeile: "<<zeile<<"\n";
	};
	return;
}

void KommaGegenPunkt(std::string str)
{
	for(long long unsigned int i = 0; i < str.length(); i++)
	{
		if(str[i] == ',')str[i] = '.';
	}
	return;
}

void Lese_posNr(std::string str, Position &pos, int &i)
{
	std::cout<<"Lese_posNr\n";
	pos.posNr = str;
	return;
}

void Lese_kurzText(std::string str, Position &pos, int &i)
{
	std::cout<<"Lese_kurzText\n";
	pos.kurzText = str;
	return;
}

void Lese_langText(std::string str, Position &pos, int &i)
{
	std::cout<<"Lese_langText\n";
	pos.langText = str;
	return;
}


void Lese_menge(std::string str, Position &pos, int &i)
{
	std::cout<<"Lese_menge\n";
	KommaGegenPunkt(str);
	pos.menge = atof(str.c_str());
	return;
}

void Lese_einheit(std::string str, Position &pos, int &i)
{
	std::cout<<"Lese_einheit\n";
	pos.einheit = str;
	return;
}

void Lese_evtlPos(std::string str, Position &pos, int &i)
{
	std::cout<<"Lese_evtlPos\n";
	if(str.empty())
		pos.evtlPos = false;
	else
		pos.evtlPos = true;
	return;
}

void Lese_schwerPunkt(std::string str, Position &pos, int &i)
{
	std::cout<<"Lese_schwerPunkt\n";
	if(str.empty())
		pos.schwerPunkt = false;
	else
		pos.schwerPunkt = true;
	return;
}

void Lese_Bieter(std::string str, Position &pos, int &i)
{
	std::cout<<"Lese_Bieter\n";
	KommaGegenPunkt(str);
	pos.lstAngebote[i++].EP = atof(str.c_str());
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
			std::cout<<"Verarbeite: "<<zeile.substr(pos, neuPos-pos)<<"\n";
			Bieter* neuBieter = new Bieter;
			neuBieter->name = zeile.substr(pos, neuPos-pos);
			bieter.push_back(neuBieter);
		}
		stelle++;
		pos = neuPos + 1;
	}
	return;
}