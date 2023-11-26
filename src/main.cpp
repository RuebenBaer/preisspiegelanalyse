#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "lvSruct.h"

void einlesen(std::ifstream &file, std::vector<Position> &LV);
void ErsteZeileEinlesen(std::string &zeile, void (*funktionsListe[256])(std::string, Position&, int),
						int &anzLeseFunktion, std::vector<Bieter*> &lstBieter);

int main(int argc, char** argv)
{
	if(argc < 2)
	{
		std::cout<<"Verwendung:\n"<<argv[0]<<" [Datei]\n";
		system("PAUSE");
		return EXIT_FAILURE;
	}
	
	std::ifstream file;
	file.open(argv[1], std::ios::in);
	if(!file.good())
	{
		std::cout<<"Fehler beim oeffnen der Datei "<<argv[1]<<"\n";
		system("PAUSE");		
		return EXIT_FAILURE;
	}
	
	std::vector<Position> LV;
	
	std::streampos fileAnfang = file.tellg();
	
	einlesen(file, LV);
	
	file.close();
	system("PAUSE");
	return EXIT_SUCCESS;
}

void einlesen(std::ifstream &file, std::vector<Position> &LV)
{
	std::streampos fileAnfang = file.tellg();
	std::string zeile;
	
	void (*funktionsListe[256])(std::string, Position&, int);
	int anzLeseFunktion = 0;
	std::getline(file, zeile);
	
	std::vector<Bieter*> lstBieter;
	ErsteZeileEinlesen(zeile, funktionsListe, anzLeseFunktion, lstBieter);

	while(!file.eof())
	{
		std::getline(file, zeile);
		
		size_t pos = 0;
		size_t neuPos = 0;
		while(1)
		{
			neuPos = zeile.find(";", pos);
			if(neuPos == std::string::npos)
			{
				break;
			}
			std::cout<<zeile.substr(pos, neuPos-pos)<<"[semikolon]";
			pos = neuPos + 1;
		}
		std::cout<<"\n";
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

void Lese_posNr(std::string str, Position &pos, int i)
{
	pos.posNr = str;
	return;
}

void Lese_kurzText(std::string str, Position &pos, int i)
{
	pos.kurzText = str;
	return;
}

void Lese_langText(std::string str, Position &pos, int i)
{
	pos.langText = str;
	return;
}

void Lese_menge(std::string str, Position &pos, int i)
{
	KommaGegenPunkt(str);
	pos.menge = atof(str.c_str());
	return;
}

void Lese_einheit(std::string str, Position &pos, int i)
{
	pos.einheit = str;
	return;
}

void Lese_evtlPos(std::string str, Position &pos, int i)
{
	if(str.empty())
		pos.evtlPos = false;
	else
		pos.evtlPos = true;
	return;
}

void Lese_schwerPunkt(std::string str, Position &pos, int i)
{
	if(str.empty())
		pos.schwerPunkt = false;
	else
		pos.schwerPunkt = true;
	return;
}

void Lese_Bieter(std::string str, Position &pos, int i)
{
	KommaGegenPunkt(str);
	pos.lstAngebote[i].EP = atof(str.c_str());
	return;
}

void ErsteZeileEinlesen(std::string &zeile, void (*funktionsListe[256])(std::string, Position&, int),
						int &anzLeseFunktion, std::vector<Bieter*> &lstBieter)
{
	size_t pos = 0, neuPos = 0;
	int anzBieter = 0;
	anzLeseFunktion = 0;
	std::string begriff;
	do{
		neuPos = zeile.find(";", pos);
		if(neuPos == std::string::npos)
		{
			break;
		}
		begriff = zeile.substr(pos, neuPos-pos);
		std::cout<<"Zeile: "<<begriff<<"\n";
		if("posNr" == begriff)
		{
			std::cout<<"posNr gefunden\n";
			anzLeseFunktion++;
		}
		if("kurzText" == begriff)
		{
			std::cout<<"kurzText gefunden\n";
			anzLeseFunktion++;
		}
		if("langText" == begriff)
		{
			std::cout<<"langText gefunden\n";
			anzLeseFunktion++;
		}
		if("mitGP" == begriff)
		{
			std::cout<<"mitGP gefunden\n";
			anzLeseFunktion++;
		}
		if("schwerPunkt" == begriff)
		{
			std::cout<<"schwerPunkt gefunden\n";
			anzLeseFunktion++;
		}
		if("menge" == begriff)
		{
			std::cout<<"menge gefunden\n";
			anzLeseFunktion++;
		}
		if("einheit" == begriff)
		{
			std::cout<<"einheit gefunden\n";
			anzLeseFunktion++;
		}
		std::cout<<begriff.compare("Bieter:")<<"\n";
		if(begriff.compare("Bieter:") < 0)
		{
			std::cout<<"Bieter: gefunden\n";
			/*if(anzBieter > 199)continue;
			Bieter *bieter = new Bieter;
			bieter->name = begriff.substr(7, begriff.length() - 7);
			lstBieter.push_back(bieter);*/
			anzLeseFunktion++;
			anzBieter++;
		}
		pos = neuPos + 1;
	}while(neuPos != std::string::npos);
	
	int i = 0;
	do{
		neuPos = zeile.find(";", pos);
		if(neuPos == std::string::npos)
		{
			break;
		}
		begriff = zeile.substr(pos, neuPos-pos);
		if("posNr" == begriff)
		{
			funktionsListe[i] = &Lese_posNr;
			i++;
		}
		if("kurzText" == begriff)
		{
			funktionsListe[i] = &Lese_kurzText;
			i++;
		}
		if("langText" == begriff)
		{
			funktionsListe[i] = &Lese_langText;
			i++;
		}
		if("mitGP" == begriff)
		{
			funktionsListe[i] = &Lese_evtlPos;
			i++;
		}
		if("schwerPunkt" == begriff)
		{
			funktionsListe[i] = &Lese_schwerPunkt;
			i++;
		}
		if("menge" == begriff)
		{
			funktionsListe[i] = &Lese_menge;
			i++;
		}
		if("einheit" == begriff)
		{
			funktionsListe[i] = &Lese_einheit;
			i++;
		}
		if(begriff.compare("Bieter:") < 0)
		{
			funktionsListe[i] = &Lese_Bieter;
			i++;
		}
		pos = neuPos + 1;
		if(i > 55)break;
	}while(neuPos != std::string::npos);
	return;
}