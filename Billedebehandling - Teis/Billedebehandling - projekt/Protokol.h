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

	std::string header();		//Skal sørge for at lave headeren. Altså finde ud af sekvensnummeret og checkLastBit'et skal sættes til 0.
	std::string trailer();		//Skal kalde og lave et CRC-check på data og header.
	std::string packing();		//Skal kalde header() og trailer(), så de er samlet i et funktions. De skal alligevel altid kaldes sammen
	void unpacking(std::string s); //Skal tage en string der repræsenterer en pakke, og dele den op i checksum, lastbit, sekvensnummer og data.
	void unpackingNAK(std::string s); //Skal stå for at pakke NAK's ud, og returnere de data, som NAK'et indeholder.
	void setLastBit();	//Skal gå ind i stringen og sætte det rigtige bit højt.
	
	bool checkChecksum();		//Skal checke at checksummen er rigtig, og returnere true i så fald.
	bool checkLastBit();		//Skal checke om lastBit er sat højt, og returnere true hvis den er. Dette er for at vi kan vide hvornår der skal sendes NAKs
	void sequenceSpot();		//Skal sørge for at data bliver lagt på det rigtige sted i recieveBufferen
	void makeNAK(std::string);				//Skal oprette et NAK i den globale NAK-buffer som indeholder sekvensnummeret på pakken. Skal kun kaldes hvis checkCheckSum() returnerer false

	//simple get og set metoder
	std::string getChecksum();
	std::string getLastBit();
	std::string getSequenceNumber();
	std::string getRecievedData();
	void setDataSize(int dataS); //skal bruges hvis vi ønsker at ændre datastørrelsen på en protokol. Som default er datastørrelsen sat til 40.
	void setToneStart(int);
	void setToneSlut(int);
	std::string getString();
	
	~Protokol();

protected:
	std::string startString;
	std::string endString;
	int toneNrStart;
	int toneNrSlut;

	int dataSize;

	//Bruges til når data modtages, skabes af funktionen unpacking()
	std::string checksum;
	std::string lastBit;
	std::string sequenceNumber;
	std::string recievedData;
};

#endif

