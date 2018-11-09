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
	const unsigned SAMPLE_RATE = 8000;

	//Fra antagelsen om at en protokol indeholder 4 byte
	// sættes raw til 4bytes*2toner*SAMPLES * 2 protokoller
	const unsigned arraySize = 256000;
	sf::Int16 raw1[arraySize];
	sf::Int16 raw2[arraySize];
	sf::Int16 raw3[arraySize];
	sf::Int16 raw4[arraySize];

	BitDTMF sekvens("101011010001101100110100010110101100001101101", 44100, 41000, 32);
	//Format ( string, samples, samplefrekvens, protokolOpdelingsstørrelse)
	
	sekvens.toProtokol(protokoller);
	sekvens.toDTMF(protokoller, dtmfToner);
	int protStart = protokoller[1].getToneStart();
	int protSlut = protokoller[1].getToneSlut();
	
	
	std::vector<float> tone;

	for (int i = protStart; i < protSlut; i++)
	{
		tone = dtmfToner[i].createTone();
		for (size_t j = 1; j < SAMPLES + 1; j++) {
			raw1[(j * SAMPLES) - SAMPLES] = tone[j];
		}
	}


	/*tone = dtmfToner[0].createTone();
	for (size_t i = 0; i < SAMPLES; i++)
	{
		raw1[i] = tone[i];	
	}*/

	//Overwriter raw igen og igen og tildeler 
	//Laver en stor array pr. protokol objekt eller pr 
	//Predefineret antal protokoller
	//Så skal der sendes et ack hver efter et forudbestemt antal prot
	//Så behøves kun 2 arrays til at indeholde tonedata
	

	std::cout << raw1[1100] << std::endl;


	sf::SoundBuffer Buffer;
	Buffer.loadFromSamples(raw1, SAMPLES, 1, SAMPLE_RATE);
	sf::Sound Sound;
	Sound.setBuffer(Buffer);
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







//sf::SoundBuffer Buffer;
//if (!Buffer.loadFromSamples(raw, SAMPLES, 1, SAMPLE_RATE)) {
//	std::cerr << "Loading failed!" << std::endl;
//	return 1;
//}