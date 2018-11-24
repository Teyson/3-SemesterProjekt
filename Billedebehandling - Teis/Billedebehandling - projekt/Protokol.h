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
	
	bool checkChecksum();		//Skal checke at checksummen er rigtig, og returnere true i så fald.
	std::string writeChecksum();
	bool checkLastBit();		//Skal checke om lastBit er sat højt, og returnere true hvis den er. Dette er for at vi kan vide hvornår der skal sendes NAKs
	std::string getRecievedChecksum();
	std::string getRecievedLastBit();
	std::string getRecievedSequenceNumber();
	std::string getRecievedData();

	//Afsending af pakker
	std::string header();		//Skal sørge for at lave headeren. Altså finde ud af sekvensnummeret og checkLastBit'et skal sættes til 0.
	std::string trailer();		//Skal kalde og lave et CRC-check på data og header.
	std::string packing();		//Skal kalde header() og trailer(), så de er samlet i et funktions. De skal alligevel altid kaldes sammen
	void setLastBit();			//Skal gå ind i stringen og sætte det rigtige bit højt.
	void clearLastBit();		//skal sætte LB til 0, hvis LB har været sat. 
	void setResendBit();		//Skal sættes bit som fortæller om pakken er bllevet sendt før.
	std::string getData();
	std::string getSequenceNumber();
	std::string getCRCcheck();
	std::string getLastBit();
	
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
	std::string getRecievedPacket();

	~Protokol();

protected:
	std::string startString;
	std::string endString;
	int toneNrStart;
	int toneNrSlut;
	int dataSize = 40;
	int placering = 0;

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

