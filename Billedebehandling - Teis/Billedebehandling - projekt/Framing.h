#pragma once
#ifndef PROTOKOL
#define PROTOKOL

#include <string>
#include <vector>
#include "CRC.h"
//#include "BitDTMF.h" SKAL IKKE INKLUDERES


class Framing
{
public:
	Framing(std::string sr);
	Framing(std::string sr, int plac);

	//Modtagning af almindelige pakker
	void unpacking(); //Skal tage en string der repr�senterer en pakke, og dele den op i checksum, lastbit, sekvensnummer og data.
	
	bool checkChecksum();		//Skal checke at checksummen er rigtig, og returnere true i s� fald.
	std::string writeChecksum();
	bool checkLastBit();		//Skal checke om lastBit er sat h�jt, og returnere true hvis den er. Dette er for at vi kan vide hvorn�r der skal sendes NAKs
	std::string getRecievedChecksum();
	std::string getRecievedLastBit();
	std::string getRecievedSequenceNumber();
	std::string getRecievedData();

	//Afsending af pakker
	std::string header();		//Skal s�rge for at lave headeren. Alts� finde ud af sekvensnummeret og checkLastBit'et skal s�ttes til 0.
	std::string trailer();		//Skal kalde og lave et CRC-check p� data og header.
	std::string packing();		//Skal kalde header() og trailer(), s� de er samlet i et funktions. De skal alligevel altid kaldes sammen
	void setLastBit();			//Skal g� ind i stringen og s�tte det rigtige bit h�jt.
	void clearLastBit();		//skal s�tte LB til 0, hvis LB har v�ret sat. 
	void setResendBit();		//Skal s�ttes bit som fort�ller om pakken er bllevet sendt f�r.
	std::string getData();
	std::string getSequenceNumber();
	std::string getCRCcheck();
	std::string getLastBit();
	
	//Modtagning af NAK
	bool checkNAKChecksum();
	std::vector<std::string> getNAKs();

	
	//simple get og set metoder
	void setDataSize(int dataS); //skal bruges hvis vi �nsker at �ndre datast�rrelsen p� en protokol. Som default er datast�rrelsen sat til 40.
	void setToneStart(int);
	void setToneSlut(int);
	int getToneStart();
	int getToneSlut();
	std::string getString();
	std::string getRecievedPacket();
	std::string getResendBit();

	~Framing();

protected:
	std::string startString;
	std::string endString;
	int toneNrStart;
	int toneNrSlut;
	int dataSize = 40;
	int placering = 0;
	std::string beforeTrailer;

	//Bruges til n�r data modtages, skabes af funktionen unpacking()
	std::string checksum;
	std::string lastBit;
	std::string sequenceNumber;
	std::string data;
	std::string recievedPacket;

	//Bruges n�r NAK modtages
	std::string recievedNAK;
	std::vector<std::string> NAKs;
};

#endif

