#include <SFML/Audio.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <bitset>
#include <conio.h>  //kbhit
#include <thread>
#include <atomic>
#include <fstream> //Gem til fil
#include <mutex>

#include "CImg.h"
#include "PictureProcessing.h"
#include "customRecorder.h"
#include "Framing.h"
#include "BitDTMF.h"
#include "NAK.h"
#include "PacketSelection.h"
#include "TextProcessing.h"
#include "Synkronisering.h"
#include "DTMFToner.h"
#include "Bit2Tekst.h"
#include "Opdeling.h"
#include "Afspilning.h"
#include "Timer.h"


////  Gamlekonstanter  ////
int toneLength = 500;
int sampleFreqGlobal = 8000;//41000
int samplesGlobal = (sampleFreqGlobal * toneLength)/1000;//16000;//44100
int protokolOpdelingGlobal = 32;

bool wasLastNakRecieved = true;
bool end = false;
bool isResend = false;

std::string finalBitString;
int framesSend = 3;
/////////////////////////////////////





int main() {
	char answer;
	std::string userInput;
	std::string bitString;

	std::cout << "Indtast den tekststreng du vil arbejde med." << std::endl;
	std::getline(std::cin, userInput);

	TextProcessing processer(userInput);
	bitString = processer.stringToBitsString();


label:

	while (true)
	{

		std::cout << "Afsender eller Modtager? (a/m): " << std::endl;
		std::cin >> answer;
		if (answer == '1') {						// TextProcessing

		}
		else if (answer == '2') {	// ToProtokol

		}
		else if (answer == '3') { // ToDTMF

		}
		else if (answer == '4') { // Play + Sync
			Afspilning test("0110110010101001", samplesGlobal, sampleFreqGlobal);
			//	0110 1100 1010 1001


			std::vector<int>bla;
			bla.push_back(0);
			bla.push_back(2);
			bla.push_back(0);

			sf::SoundBuffer Buffer;
			if (!Buffer.loadFromSamples(test.playThis(bla), test.getarraySize(), 1, sampleFreqGlobal)) {
				std::cerr << "Loading failed!" << std::endl;
				return 1;
			}

			sf::Sound Sound;
			Sound.setBuffer(Buffer);
			Sound.play();
			while (1) {
				sf::sleep(sf::milliseconds(100));
			}
		}
		else if (answer == '5') { // NAK

		}
		else {
			char answer;
			goto label;

		}
	}


Ending:

std::cout << "Transmission afsluttet" << std::endl;
	
	int c;
	std::cin >> c;

	return 0;
}







//sf::SoundBuffer Buffer;
//if (!Buffer.loadFromSamples(raw, SAMPLES, 1, SAMPLE_RATE)) {
//	std::cerr << "Loading failed!" << std::endl;
//	return 1;
//}