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
    sf::Int16* playForrigePakker();

    //	Hjælpefunktioner	//
    int adddatapakke(int pakke, int abc);
    int makeSyncSequence();
    void clearRaw1DTMF();
    void setLastBit();

    //	Get-funktioner	
    unsigned int getarraySize();
    int getAntalDataPakker();
    int getAntalDTMFToner();
    int getAntalElementerIArray();

    ~Afspilning();
protected:
    int samples;
    int sampleFreq;
    BitDTMF sekvens;
    std::vector<int> forrigeSendteDatapakker;
    std::vector<Protokol>datapakker;
    std::vector<DTMFToner> dtmfToner;
    unsigned int arraySize = 224000;
    unsigned int elementerIArray;
    sf::Int16* raw1; // en datapakke
};

