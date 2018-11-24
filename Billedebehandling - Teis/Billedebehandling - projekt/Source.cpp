#include <SFML/Audio.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <bitset>
#include <conio.h>  //kbhit

#include "CImg.h"
#include "PictureProcessing.h"
//#include "customRecorder.h"
#include "Protokol.h"
#include "BitDTMF.h"
#include "DTMFToner.h"
#include "Bit2Tekst.h"


std::vector<Protokol> protokoller;

std::vector<DTMFToner> dtmfToner;
//Husk at s�tte samplerate og antal samples inde i klassen

std::vector<float> sampleToner;

int main() {

	std::string en8BitS = "01100010"; //b
	std::string en8BitS2 = "01100001"; //a 
	std::string en8BitS3 = "01100101"; //e

	std::string en8BitS4 = en8BitS2 + en8BitS + en8BitS3;


	Bit2Tekst haha(en8BitS4);
	std::cout << "laengde: " << en8BitS4.size() / 8 << std::endl;

	std::cout << "skal returnerer et bogstav: " << haha.bitToString() << std::endl;


	std::string j = "";

	//char c1 = static_cast<char>(std::stoi(en8BitS, nullptr, 2) + 64);
	//std::string s = c1;
	//std::cout << "skal returnerer et bogstav test: " << s << std::endl;
	char c2 = static_cast<char>(std::bitset<6>(en8BitS2).to_ulong() + 64);;
	std::cout << "skal returnerer et bogstav test: " << unsigned(c2) << std::endl;


	/*
	const unsigned SAMPLES = 16000;
	const unsigned SAMPLE_RATE = 8000;

	//Fra antagelsen om at en protokol indeholder 4 byte
	// sættes raw til 4bytes*2toner*SAMPLES * 2 protokoller
	const unsigned arraySize = 256000;
	sf::Int16 raw1[arraySize];

	BitDTMF sekvens("11101111110010101101000110110011010001011010", 44100, 41000, 32);
	//Format ( string, samples, samplefrekvens, protokolOpdelingsstørrelse)
	
	sekvens.toProtokol(protokoller);
	sekvens.toDTMF(protokoller, dtmfToner);
	int protStart = protokoller[0].getToneStart();
	int protSlut = protokoller[0].getToneSlut();
	

	std::vector<float> tone;
	/*tone = dtmfToner[0].createTone();
	
	for (int i = 0; i < SAMPLES; i++)
	{
		raw1[i] = tone[i];
	}

	for (int i = protStart + 1; i < protSlut + 1; i++)
	{
		tone = dtmfToner[i - 1].createTone();

		for (int k = 0, j = ((SAMPLES * i) - SAMPLES); k < SAMPLES; j++, k++) {
			raw1[j] = tone[k];
		}
	}
	j-loopet appender alle toner i en protokol til raw1 array. k-loopet kører
	alle elementer igennem i tone-vektoren.

	Overwriter raw igen og igen og tildeler 
	Laver en stor array pr. protokol objekt eller pr 
	Predefineret antal protokoller
	Så skal der sendes et ack hver efter et forudbestemt antal prot
	Så behøves kun 2 arrays til at indeholde tonedata
	



	sf::SoundBuffer Buffer;
	if (!Buffer.loadFromSamples(raw1, arraySize, 1, SAMPLE_RATE)) {
		std::cerr << "Loading failed!" << std::endl;
		return 1;
	}
	sf::Sound Sound;
	Sound.setBuffer(Buffer);
	Sound.play();
	while (1) {
		sf::sleep(sf::milliseconds(100));
	}


	Custom recorder
	if (!customRecorder::isAvailable())
	{
		std::cout << "Audio capture not available";
		return 0;
	}

	customRecorder recorder;

	recorder.start(8000);					//Start recording
	std::cout << "Recording...." << std::endl;

	while (!_kbhit())
	{
		std::cout << recorder.getVectorSize() << std::endl;
	}

	recorder.stop();						//Stop recording
	std::cout << "end recording" << std::endl;


	/*for (int i = 0; i < recorder.getVectorSize(); i++)
	{
		std::cout << recorder.getVector(i) << std::endl;
	}
	*/
	



	int c;
	std::cin >> c;

	return 0;
}







//sf::SoundBuffer Buffer;
//if (!Buffer.loadFromSamples(raw, SAMPLES, 1, SAMPLE_RATE)) {
//	std::cerr << "Loading failed!" << std::endl;
//	return 1;
//}