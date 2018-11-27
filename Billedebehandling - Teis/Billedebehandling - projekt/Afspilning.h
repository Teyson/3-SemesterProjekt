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

//	tilføjer datapakker eller strings direkte til afspilningsarrayet//
	void playThis(std::vector<int> resendPackage);
	void playString(std::string nakString);
	void playSequence(int start, int antal);
	
//	Hjælpefunktioner	//
	int adddatapakke(int pakke,int abc);
	int makeSyncSequence(int perioder);
	void clearRaw1DTMF();
	

//	Get-funktioner	
	unsigned int getarraySize();

//	Afspiller	//
	void playRaw1();
	~Afspilning();
protected:
	int samples;
	int sampleFreq;
	BitDTMF sekvens;
	std::vector<Framing>datapakker;
	std::vector<DTMFToner> dtmfToner;
	unsigned int arraySize=25600;
	sf::Int16* raw1; // [arraySize]
	sf::SoundBuffer Buffer;
	sf::Sound Sound;


};

