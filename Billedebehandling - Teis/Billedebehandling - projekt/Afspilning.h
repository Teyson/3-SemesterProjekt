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
	Afspilning(std::string datainput, int samplesInput, int sampleRateInput,int start, int antal);
	bool ichBinEinIdiot(int sekvensen, int start, int antal);
	sf::Int16* startplaying();
	unsigned int getarraySize();
	~Afspilning();
protected:
	int samples;
	int sampleFreq;
	std::vector<Protokol>datapakker;
	std::vector<DTMFToner> dtmfToner;
	unsigned int arraySize=256000;
	sf::Int16 raw1[500000]; // [arraySize]


};

