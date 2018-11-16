#pragma once
#ifndef BIT2DTMF
#define BIT2DTMF

#include <string>
#include <iostream>
#include <bitset>
#include <vector>

#include "Protokol.h"
#include "DTMFToner.h"

class BitDTMF
{
public:
	BitDTMF();
	BitDTMF(std::string s, int antalSamples, int samplingFrekvens, int opdelingsSize);
	
	void toProtokol(std::vector<Protokol>&);
	//toProtokol - Del datastringen op i bidder af objekter

	void toDTMF(std::vector<Protokol>&, std::vector<DTMFToner>&);
	//toDTMF - Deler protokolstrengen op i tonebidder og laver den om til toner

	~BitDTMF();

protected:
	int antalSamples; //Antal samples
	int fs; //Sampling frekvens
	int Ts; //
	unsigned int stringLength;
	int opdelingsSize;
	int toneNr;
	std::string startString;
	

public:
	int antalProtokolStrings;


	//Returnerning af array https://stackoverflow.com/questions/3473438/return-array-in-a-function
};

#endif
