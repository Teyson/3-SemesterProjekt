#pragma once
#include <vector>
#include <string>
#include <SFML/Audio.hpp>

#include "Framing.h"
#include "BitDTMF.h"
#include "DTMFToner.h"
class Afspilning
{
public:
	Afspilning();

//	på bagrund af datainputet deler denne funktion den op i datapakker samt framer datapakkerne	//
	Afspilning(std::string datainput, int samplesInput, int sampleRateInput);

//	Afspiller datapakker eller strings direkte //
	sf::Int16* playThis(std::vector<int> resendPackage);
	sf::Int16* playString(std::string nakString);
	sf::Int16* playSequence(int start, int antal);
	
//	Hjælpefunktioner	//
	int adddatapakke(int pakke,int abc);
	int makeSyncSequence(int perioder);
	void clearRaw1DTMF();

//	Get-funktioner	
	unsigned int getarraySize();
	
	~Afspilning();
protected:
	int samples;
	int sampleFreq;
	BitDTMF sekvens;
	std::vector<Framing>datapakker;
	std::vector<DTMFToner> dtmfToner;
	unsigned int arraySize=256000;
	sf::Int16* raw1; // [arraySize]


};

