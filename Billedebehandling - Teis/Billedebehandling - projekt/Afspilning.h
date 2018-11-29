#pragma once
#include <vector>
#include <string>
#include <SFML/Audio.hpp>

#include "Protokol.h"
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
	int adddatapakke(int pakke,int abc,int rawµ);
	int makeSyncSequence(int rawµ);
	void clearRaw1DTMF();
	void setLastBit();

//	Get-funktioner	
	unsigned int getarraySize();
	int getAntalDataPakker();
	
	~Afspilning();
protected:
	int samples;
	int sampleFreq;
	BitDTMF sekvens;
	std::vector<Protokol>datapakker;
	std::vector<DTMFToner> dtmfToner;
	unsigned int arraySize=2240;
	int antalSyncvaerdier = 6400;
	sf::Int16* raw1; // en datapakke
	sf::Int16* raw2;
	sf::Int16* raw3;
	sf::Int16* raw4;
	sf::Int16* raw5;
	sf::Int16* raw6;
	sf::Int16* rawNak;
	int rawNumber=-1;


};

