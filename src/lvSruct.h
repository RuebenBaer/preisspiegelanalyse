#ifndef __LV_H_
#define __LV_H_

#include <string>


typedef struct{
	std::string name;
}Bieter;

typedef struct{
	int bieterNr;
	std::string analyse;
	float EP;
}Angebot;

typedef struct{
	std::string posNr;
	std::string kurzText;
	std::string langText;
	float menge;
	std::string einheit;
	int evtlPos;
	int risiko;
	Angebot *lstAngebote;
	int anzAngebote;
	double median;
}Position;

#endif //__LV_H_