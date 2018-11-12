#include <SFML/Audio.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <bitset>
#include <conio.h>  //kbhit
#include <fstream> //Gem til fil

#include "CImg.h"
#include "PictureProcessing.h"
#include "customRecorder.h"
#include "Protokol.h"
#include "BitDTMF.h"

//#include "Behandling.h"



std::vector<Protokol> protokoller;

std::vector<DTMFToner> dtmfToner;
//Husk at sætte samplerate og antal samples inde i klassen

std::vector<sf::Int16> sampleToner;


int main() {
	const unsigned SAMPLES = 32000;
	const unsigned SAMPLE_RATE = 8000;
	const unsigned AMPLITUDE = 10000;

	sf::Int16 raw[SAMPLES];
	/*
	BitDTMF sekvens("1010100100100101", 44100, 41000, 5);
	sekvens.toProtokol(protokoller);
	sekvens.toDTMF(protokoller, dtmfToner);


	}

	sf::SoundBuffer Buffer;
	if (!Buffer.loadFromSamples(raw, SAMPLES, 1, SAMPLE_RATE)) {
		std::cerr << "Loading failed!" << std::endl;
		return 1;
	}

	sf::Sound Sound;
	Sound.setBuffer(Buffer);
	Sound.setLoop(true);
	Sound.play();
	while (1) {
		sf::sleep(sf::milliseconds(100));
	}*/


	//Custom recorder
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
		//std::cout << recorder.getVectorSize() << std::endl;
	}

	recorder.stop();						//Stop recording
	std::cout << "end recording" << std::endl;



	/*std::vector<sf::Int16> a;
	for (int i = 0; i < recorder.getVectorSize(); i++)
	{
		a.push_back(recorder.getVector(i));
	}

	for (int i = 0; i < SAMPLES; i < i++)
	{
		raw[i] = a[i];
	}

	sf::SoundBuffer Buffer;
	Buffer.loadFromSamples(raw, SAMPLES, 1, SAMPLE_RATE);
	sf::Sound Sound;
	Sound.setBuffer(Buffer);
	Sound.setLoop(true);
	Sound.play();
	while (1) {
		sf::sleep(sf::milliseconds(100));
	}*/


	/*ofstream outdata;
	outdata.open("komnumand.csv");

	for (int i = 0; i < recorder.getVectorSize(); i++)
	{
	outdata << recorder.getVector(i) << endl;
	}
	outdata.close();*/

	/*for (int i = 0; i < recorder.getVectorSize(); i++)
	{
		std::cout << recorder.getVector(i) << std::endl;
	
	}*/

	 
	
	int c;
	std::cin >> c;

	return 0;
}