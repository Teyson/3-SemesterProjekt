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


std::vector<Protokol> protokoller;

std::vector<DTMFToner> dtmfToner;
//Husk at s�tte samplerate og antal samples inde i klassen

std::vector<float> sampleToner;

int main() {
	const unsigned SAMPLES = 16000;
	const unsigned HEY = 80000;
	const unsigned SAMPLE_RATE = 8000;
	sf::Int16 raw[HEY];

	BitDTMF sekvens("10101101000110110101", 44100, 41000, 20);
	//Format ( string, samples, samplefrekvens, protokolOpdelingsstørrelse)
	
	sekvens.toProtokol(protokoller);
	sekvens.toDTMF(protokoller, dtmfToner);
	int antalToner = dtmfToner.size();
	
	std::vector<float> tone;
	tone = dtmfToner[2].createTone();
	for (size_t i = 0; i < SAMPLES; i++)
	{
		raw[i] = tone[i];
	}

	/*for (int j = 0; j < antalToner; j++)
	{
		tone = dtmfToner[j].createTone();
		for (int i = 0; i < SAMPLES; i++)
		{
			raw[((SAMPLES*(j+1))-SAMPLES)+i] = tone[i];
		}
	}*/
	std::cout << raw[1100] << std::endl;


	sf::SoundBuffer Buffer;
	if (!Buffer.loadFromSamples(raw, SAMPLES, 1, SAMPLE_RATE)) {
		std::cerr << "Loading failed!" << std::endl;
		return 1;
	}

	sf::Sound Sound;
	Sound.setBuffer(Buffer);
	Sound.setLoop(false);
	Sound.play();
	while (1) {
		sf::sleep(sf::milliseconds(100));
	}


	//Custom recorder
	//if (!customRecorder::isAvailable())
	//{
	//	std::cout << "Audio capture not available";
	//	return 0;
	//}

	//customRecorder recorder;

	//recorder.start(8000);					//Start recording
	//std::cout << "Recording...." << std::endl;

	//while (!_kbhit())
	//{
	//	std::cout << recorder.getVectorSize() << std::endl;
	//}

	//recorder.stop();						//Stop recording
	//std::cout << "end recording" << std::endl;


	///*for (int i = 0; i < recorder.getVectorSize(); i++)
	//{
	//	std::cout << recorder.getVector(i) << std::endl;
	//}*/

	//


	int c;
	std::cin >> c;

	return 0;
}