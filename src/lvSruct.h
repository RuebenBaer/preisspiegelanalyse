#ifndef __LV_H_
#define __LV_H_

#include <string>


typedef struct{
	std::string name;
}Bieter;

typedef struct{
	int bieterNr;
	int cAnalyse = 0;
	/*cAnalyse
	0. bit Preis hoch = 1, Preis niedrig = 0;
	1. - 3. …, sehr, extrem;
	4. bit Evtl. Pos oder Preisabfrage ja = 1, nein = 0;
	5. - 7. Kostenpotetial moderat, hoch, sehr hoch;
	*/
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