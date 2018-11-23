#include <SFML/Audio.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <bitset>
#include <conio.h>  //kbhit
#include <fstream> //Gem til fil
#include <mutex>
#include <thread>

#include "CImg.h"
#include "PictureProcessing.h"
#include "customRecorder.h"
#include "Protokol.h"
#include "BitDTMF.h"
#include "DTMFToner.h"
#include "Synkronisering.h"

//#include "Behandling.h"



std::vector<Protokol> protokoller;

std::vector<DTMFToner> dtmfToner;
//Husk at s�tte samplerate og antal samples inde i klassen

std::vector<float> sampleToner;


int main() {
	const unsigned SAMPLES = 8000;
	const unsigned SAMPLE_RATE = 8000;

	//Fra antagelsen om at en protokol indeholder 4 byte
	// sættes raw til 4bytes*2toner*SAMPLES * 2 protokoller
	const unsigned arraySize = 256000;
	sf::Int16 raw1[arraySize];

	BitDTMF sekvens("0000", 44100, 41000, 8);
	//Format ( string, samples, samplefrekvens, protokolOpdelingsstørrelse)

	sekvens.toProtokol(protokoller);
	sekvens.toDTMF(protokoller, dtmfToner);
	int protStart = protokoller[0].getToneStart();
	int protSlut = protokoller[0].getToneSlut();



	////Enkel Tone
	std::vector<float> tone;
	//tone = dtmfToner[0].createTone();
	//std::cout << dtmfToner[0].getToneNumber() << std::endl;
	//for (size_t i = 0; i < SAMPLES; i++)
	//{
	//	raw1[i] = tone[i];
	//}

	/*tone = dtmfToner[0].createTone();

	for (int i = 0; i < SAMPLES; i++)
	{
		raw1[i] = tone[i];
	}*/

	for (int i = protStart + 1; i < protSlut + 1; i++)
	{
		tone = dtmfToner[i - 1].createTone();

		for (int k = 0, j = ((SAMPLES * i) - SAMPLES); k < SAMPLES; j++, k++) {
			raw1[j] = tone[k];
		}
	}
	




	/*sf::SoundBuffer Buffer;
	Buffer.loadFromSamples(raw1, arraySize, 1, SAMPLE_RATE);

	sf::Sound Sound;
	Sound.setBuffer(Buffer);
	Sound.play();
	while (1) {
		sf::sleep(sf::milliseconds(100));
	}*/



	customRecorder recorder;
	recorder.start(8000);					//Start recording  
	std::cout << "Recording...." << std::endl;

	sf::sleep(sf::milliseconds(100));

	
	recorder.startThread();


   	while (!_kbhit())
	{}

	recorder.stop();						//Stop recording
	std::cout << "end recording" << std::endl;


	///*for (int i = 0; i < recorder.getVectorSize(); i++)
	//{
	//	std::cout << recorder.getVector(i) << std::endl;
	//}*/

	//


	int c;
	std::cin >> c;

	return 0;
}







//sf::SoundBuffer Buffer;
//if (!Buffer.loadFromSamples(raw, SAMPLES, 1, SAMPLE_RATE)) {
//	std::cerr << "Loading failed!" << std::endl;
//	return 1;
//}