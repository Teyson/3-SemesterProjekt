#include <SFML/Audio.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <bitset>
#include <conio.h>  //kbhit

//#include "CImg.h"
#include "PictureProcessing.h"
#include "customRecorder.h"
#include "Protokol.h"
#include "BitDTMF.h"
#include "TextProcessing.h"


std::vector<Protokol> protokoller;

std::vector<DTMFToner> dtmfToner;
//Husk at sætte samplerate og antal samples inde i klassen

std::vector<sf::Int16> sampleToner;

int main() {
	std::string newString = "Hej mit navn er Teis, jeg bor i Danmark.";

	TextProcessing TextProcesser(newString);

	std::cout << "den inputtede string er: " << TextProcesser.getInputString() << std::endl;

	std::cout << std::endl << "Bogstavernes binære værdier: " << std::endl;
	std::vector<std::bitset<8>> bitVector = TextProcesser.stringToBits();

	for (int i = 0; i < newString[i]; i++)
	{
		std::cout << bitVector[i] << std::endl;
	}

	TextProcesser.bitsetsVectorToString(bitVector);

	std::cout << std::endl << "Den lange outputString bliver så: " << std::endl 
		<< TextProcesser.getOutputString() << std::endl;

	int a = 0;
	

	//const unsigned SAMPLES = 44100;
	//const unsigned SAMPLE_RATE = 44100;
	//const unsigned AMPLITUDE = 10000;

	//sf::Int16 raw[SAMPLES];

	//BitDTMF sekvens("1010100100100101", 44100, 41000, 5);
	//sekvens.toProtokol(protokoller);
	//sekvens.toDTMF(protokoller, dtmfToner);


	//1 + 2 + 3 + 4 + 5;

	//const double TWO_PI = 6.28318;
	//const double increment1 = 697. / 44100;
	//const double increment2 = 1209. / 44100;
	//double x1 = 0;
	//double x2 = 0;
	//for (unsigned i = 0; i < SAMPLES; i++) {
	//	raw[i] = AMPLITUDE * (sin(x1*TWO_PI) + sin(x2*TWO_PI));
	//	x1 += increment1;
	//	x2 += increment2;


	//}

	//sf::SoundBuffer Buffer;
	//if (!Buffer.loadFromSamples(raw, SAMPLES, 1, SAMPLE_RATE)) {
	//	std::cerr << "Loading failed!" << std::endl;
	//	return 1;
	//}

	//sf::Sound Sound;
	//Sound.setBuffer(Buffer);
	//Sound.setLoop(true);
	//Sound.play();
	//while (1) {
	//	sf::sleep(sf::milliseconds(100));
	//}


	////Custom recorder
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
	//}

	//recorder.stop();						//Stop recording
	//std::cout << "end recording" << std::endl;

	return 0;
}