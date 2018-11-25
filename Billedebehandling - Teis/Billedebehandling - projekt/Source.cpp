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
#include "Protokol.h"
#include "BitDTMF.h"
#include "NAK.h"
#include "PacketSelection.h"
#include "TextProcessing.h"
#include "Synkronisering.h"
#include "DTMFToner.h"
#include "Bit2Tekst.h"

#include "Afspilning.h"
#include "Timer.h"



std::vector<Protokol> protokoller;

int samplesGlobal = (8000 * 20)/1000;//16000;//44100
int sampleFreqGlobal = 8000;//41000
int protokolOpdelingGlobal = 32;

std::string finalBitString;

////////// Timer //////////
bool torbenTester = true;
std::chrono::system_clock::time_point start;

void countdown() {
	
	while (torbenTester) {
		std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
		std::chrono::duration<double> differens = end - start;
		std::cout << differens.count() << std::endl;
		if (differens.count() > 1.5) {
			torbenTester = false;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}
//////////  Timer end //////////////


int main() {
	char answer;
label:
	std::cout << "Afsender eller Modtager? (a/m): " << std::endl;
	std::cin >> answer;
	if (answer == 'a') {						// Afsender
		//Afspilning test("1010101000011011111111100001101011100100",samplesGlobal,sampleFreqGlobal);
					//	0110 1100 1010 1001
	
		std::string str = "Jeg holder af at spise kage!";
		TextProcessing str_txt(str);
		std::string str_bits=str_txt.stringToBitsString();
		Bit2Tekst aben_b(str_bits);
		std::cout << aben_b.bitToString() << std::endl;

		Afspilning reA(str_bits, samplesGlobal, sampleFreqGlobal);

		
	//// Modtager side ////
		//NAK bob;
		//bob.reset();
		//bob.insertIntoArray("0000");
		//bob.insertIntoArray("0010");

		//std::cout << bob.createNAK() << std::endl;
	//// Senderside	////
		//Protokol f(bob.createNAK());
		//PacketSelection a;

		//std::vector<int>bla;
		//bla = a.selectPackets(f.getNAKs());
	//	bla.push_back(1);
	//	bla.push_back(0);
	//	bla.push_back(0);
		
		//std::cout << bla.size() << std::endl;
		/*
		start = std::chrono::system_clock::now();
		std::thread work(countdown);
		sf::sleep(sf::milliseconds(1600));
		std::cout << torbenTester << std::endl;
		*/

		sf::SoundBuffer Buffer;
		if (!Buffer.loadFromSamples(reA.playString(str_bits), reA.getarraySize(), 1, sampleFreqGlobal)) {
			std::cerr << "Loading failed!" << std::endl;
			return 1;
		}
		
		sf::Sound Sound;
		Sound.setBuffer(Buffer);
		Sound.play();
		while (1) {
			sf::sleep(sf::milliseconds(100));
		}
		//work.join();
	}
	else if (answer == 'm') {					// Modtager
		//Customrecorder
		NAK nak;
		customRecorder recorder;

		recorder.start(8000);					//Start recording
		std::cout << "Recording...." << std::endl;
		std::string modtaget =  recorder.startThread();
		recorder.stop();

		//modtaget skal opdeles i tre pakker
		bool sendNak = false;

		Protokol prot1(modtaget1);
		Protokol prot2(modtaget2);
		Protokol prot3(modtaget3);

		if (prot1.checkChecksum())
		{
			nak.insertIntoArray(prot1.getRecievedSequenceNumber());

			if (prot1.checkLastBit())
			{
				sendNak = true;
			}
		}

		if (prot2.checkChecksum())
		{
			nak.insertIntoArray(prot2.getRecievedSequenceNumber());

			if (prot2.checkLastBit())
			{
				sendNak = true;
			}
		}

		if (prot3.checkChecksum())
		{
			nak.insertIntoArray(prot3.getRecievedSequenceNumber());

			if (prot3.checkLastBit())
			{
				sendNak = true;
			}
		}
		
		if (sendNak)
		{
			std::string nakToSend = nak.createNAK();

			Afspilning nakAfspilning(nakToSend, samplesGlobal, sampleFreqGlobal);

			nakAfspilning.playString(nakToSend);
		}
	}

	else {
		char answer;
		goto label;
		
	}

		
	


	int c;
	std::cin >> c;

	return 0;
}







//sf::SoundBuffer Buffer;
//if (!Buffer.loadFromSamples(raw, SAMPLES, 1, SAMPLE_RATE)) {
//	std::cerr << "Loading failed!" << std::endl;
//	return 1;
//}