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
	Afspilning(std::string datainput, int samplesInput, int sampleRateInput);
	//bool ichBinEinIdiot(int sekvensen, int start, int antal);
	sf::Int16* playThis(std::vector<int> resendPackage);
	sf::Int16* playString(std::string nakString);
	sf::Int16* playSequence(int start, int antal);
	int adddatapakke(int pakke, int abc);
	int makeSyncSequence(int perioder);
	void clearRaw1DTMF();
	unsigned int getarraySize();
	std::vector<Framing> getDatapakkerArray();
	~Afspilning();
protected:
	int samples;
	int sampleFreq;
	BitDTMF sekvens;
	std::vector<Framing>datapakker;
	std::vector<DTMFToner> dtmfToner;
	unsigned int arraySize = 11200;
	sf::Int16 raw1[500000]; // [arraySize]


};

