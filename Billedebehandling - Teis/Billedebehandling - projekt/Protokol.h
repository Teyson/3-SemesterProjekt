#pragma once
#ifndef PROTOKOL
#define PROTOKOL

#include <string>
#include <vector>
//#include "BitDTMF.h" SKAL IKKE INKLUDERES


class Protokol
{
public:
	Protokol();
	Protokol(std::string);
	//Protokol - Datadelen til protokollen

	std::string header();		//Skal s�rge for at lave headeren. Alts� finde ud af sekvensnummeret og checkLastBit'et skal s�ttes til 0.
	std::string trailer();		//Skal kalde og lave et CRC-check p� data og header.
	std::string packing();		//Skal kalde header() og trailer(), s� de er samlet i et funktions. De skal alligevel altid kaldes sammen
	void unpacking(std::string s); //Skal tage en string der repr�senterer en pakke, og dele den op i checksum, lastbit, sekvensnummer og data.
	void unpackingNAK(std::string s); //Skal
	std::string setLastBit();	//Skal g� ind i stringen og s�tte det rigtige bit h�jt.
	bool checkChecksum();		//Skal checke at checksummen er rigtig, og returnere true i s� fald.
	bool checkLastBit();		//Skal checke om lastBit er sat h�jt, og returnere true hvis den er. Dette er for at vi kan vide hvorn�r der skal sendes NAKs
	void sequenceSpot();		//Skal s�rge for at data bliver lagt p� det rigtige sted i recieveBufferen
	void makeNAK();				//Skal oprette et NAK i den globale NAK-buffer som indeholder sekvensnummeret p� pakken. Skal kun kaldes hvis checkCheckSum() returnerer false

	void setToneStart(int);
	void setToneSlut(int);
	std::string getString();
	
	~Protokol();

protected:
	std::string startString;
	std::string endString;
	int toneNrStart;
	int toneNrSlut;

	//Bruges til n�r data modtages, skabes af funktionen unpacking()
	std::string checksum;
	std::string lastBit;
	std::string sequenceNumber;
	std::string recievedData;
};

#endif

