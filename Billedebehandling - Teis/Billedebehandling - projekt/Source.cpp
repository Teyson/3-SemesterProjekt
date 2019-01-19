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
#include <stdlib.h>

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
		
		std::cout << std::endl << "Hvilken del af koden oenskes vist?" << std::endl << "Tast 1 for TextProcessing." << std::endl << "Tast 2 for ToProtokol, som indeholder opdeling, Framing og CRC." << std::endl << "Tast 3 for Afspilning og Synkronisering." << std::endl << "Tast 5 for NAK." << std::endl;
		std::cin >> answer;
		if (answer == '1') {						// TextProcessing
			system("CLS");							// Tømmer kommandoprompten
			TextProcessing processer2(userInput);
			std::cout << "Du har valgt TextProcessing." << std::endl;

			std::cout << std::endl << "Den string der arbejdes med er: " << "\"" << userInput << "\"" << std::endl;
			std::cout << "Naar teksten er omsat til bits gennem TextProcessing bliver den: " << processer2.stringToBitsString()<< std::endl;
			
			std::cout << std::endl;
			system("pause");
		}
		else if (answer == '2') {	// ToProtokol
			system("CLS");							// Tømmer kommandoprompten

			std::cout << std::endl;
			system("pause");
		}
		else if (answer == '3') { // ToDTMF
			system("CLS");							// Tømmer kommandoprompten

			std::cout << std::endl;
			system("pause");
		}
		else if (answer == '4') { // Play + Sync
			system("CLS");							// Tømmer kommandoprompten
			Afspilning test(bitString, samplesGlobal, sampleFreqGlobal);
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

			std::cout << std::endl;
			system("pause");
		}
		else if (answer == '5') { // NAK
			system("CLS");							// Tømmer kommandoprompten

			std::cout << "NAK er valgt." << std::endl << std::endl;

			NAK nak;

			nak.updatePointerExpected();

			std::cout << "frame 0, 1 og 2 sendes, og alle modtages korrekt" << std::endl;

			nak.insertIntoArray("0000", "1010101010111010101010111011010010110001");
			nak.insertIntoArray("0001", "1010101111101010010010011101100101010100");
			nak.insertIntoArray("0010", "1010010110001101001010110110100100101101");

			std::string createdNak = nak.createNAK();

			std::cout << "Det NAK der bliver lavet er saa: " << createdNak << std::endl << std::endl;;
			std::cout << "Altsaa et tomt NAK. Derfor sendes frame 3, 4, 5. Frame 5 gaar tabt." << std::endl;

			sf::sleep(sf::Time(sf::seconds(4)));

			nak.updatePointerExpected();

			nak.insertIntoArray("0011", "0100101000111010110011010100101001011101");
			nak.insertIntoArray("0100", "0100101001010010010110101010101011010101");

			createdNak = nak.createNAK();

			std::cout << "Det NAK der bliver lavet er saa: " << createdNak << std::endl << std::endl;
			std::cout << "Altsaa et NAK paa frame 5. Frame 5 gensendes og modtages." << std::endl;

			sf::sleep(sf::Time(sf::seconds(4)));

			nak.insertIntoArray("0101", "0100101001001001000000000000000111111001");

			createdNak = nak.createNAK();

			std::cout << "Det NAK der bliver lavet er saa: " << createdNak << std::endl << std::endl;
			std::cout << "Altsaa et tomt NAK, derfor sendes frame 6, 7, 8. Frame 6 og 7 gaar tabt" << std::endl;

			sf::sleep(sf::Time(sf::seconds(4)));

			nak.updatePointerExpected();

			nak.insertIntoArray("1000", "0111101110111111111111111101010101010001");

			createdNak = nak.createNAK();

			sf::sleep(sf::Time(sf::seconds(4)));

			std::cout << "Det NAK der bliver lavet er saa: " << createdNak << std::endl << std::endl;
			std::cout << "Altsaa et NAK paa frame 6 og 7. Framesne gensendes og modtages" << std::endl;

			nak.insertIntoArray("0110", "0110011001110100101101101001010101010110");
			nak.insertIntoArray("0111", "0101001010001010010001100101011101010111");

			createdNak = nak.createNAK();

			sf::sleep(sf::Time(sf::seconds(4)));

			std::cout << "Det NAK der bliver lavet er saa: " << createdNak << std::endl << std::endl;
			std::cout << "Altsaa et tom NAK." << std::endl;

			std::cout << std::endl;
			system("pause");
		}
		else {
			char answer;
			goto label;

		}
		system("CLS");
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