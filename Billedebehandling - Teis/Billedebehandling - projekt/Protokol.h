#pragma once
#ifndef PROTOKOL
#define PROTOKOL

#include <string>
#include <vector>
#include "CRC.h"
//#include "BitDTMF.h" SKAL IKKE INKLUDERES


class Protokol
{
public:
	Protokol(std::string sr);
	Protokol(std::string sr, int plac);

	//Modtagning af almindelige pakker
	void unpacking(); //Skal tage en string der repræsenterer en pakke, og dele den op i checksum, lastbit, sekvensnummer og data.
	void makeNAK(std::vector<std::string> NAKvec);	//Skal oprette et NAK i den globale NAK-buffer som indeholder sekvensnummeret på pakken. Skal kun kaldes hvis checkCheckSum() returnerer false
													//Mangler stadig at blive skrevet, men skal bruge en recieve-buffer på for at virke. Denne buffer er ikke lavet endnu.
	bool checkChecksum();		//Skal checke at checksummen er rigtig, og returnere true i så fald.
	bool checkLastBit();		//Skal checke om lastBit er sat højt, og returnere true hvis den er. Dette er for at vi kan vide hvornår der skal sendes NAKs
	std::string getRecievedChecksum();
	std::string getRecievedLastBit();
	std::string getSequenceNumber();
	std::string getRecievedData();

	//Afsending af pakker
	std::string header();		//Skal sørge for at lave headeren. Altså finde ud af sekvensnummeret og checkLastBit'et skal sættes til 0.
	std::string trailer();		//Skal kalde og lave et CRC-check på data og header.
	std::string packing();		//Skal kalde header() og trailer(), så de er samlet i et funktions. De skal alligevel altid kaldes sammen
	void setLastBit();			//Skal gå ind i stringen og sætte det rigtige bit højt.
	
	//Modtagning af NAK
	bool checkNAKChecksum();
	std::vector<std::string> getNAKs();
	
	//simple get og set metoder
	int getToneStart();
	int getToneSlut();
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
	int placering;

	//Bruges til når data modtages, skabes af funktionen unpacking()
	std::string checksum;
	std::string lastBit;
	std::string sequenceNumber;
	std::string data;
	std::string recievedPacket;

	//Bruges når NAK modtages
	std::string recievedNAK;
	std::vector<std::string> NAKs;
};

#endif

