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
#include "Opdeling.h"
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
	else if (answer == 'm') {	// Modtager
	Modtager:

		NAK nak;
		customRecorder recorder;

		recorder.start(8000);					//Start recording
		std::cout << "Recording...." << std::endl;
		std::string modtaget =  recorder.startThread();
		recorder.stop();

		bool sendNak = false;
		
		Opdeler in(modtaget);

		int antalBitPrFrame = 56;
		int antalOpdelinger = std::stoi(in.opdel(antalBitPrFrame)[0]);

		std::vector<Protokol> modtagetProt;

		//flyttes eventuelt til Protokol
		if (antalOpdelinger > 0)
		{
			for (int i = 1; i < antalOpdelinger + 1; i++)
			{
				std::string modtagetString = in.opdel(antalOpdelinger)[i];

				Protokol prot1(modtagetString);
				modtagetProt.push_back(prot1);
			}

			for (int i = 0; i < modtagetProt.size(); i++)
			{
				Protokol prot = modtagetProt[i];
				if (prot.checkChecksum())
				{
					nak.insertIntoArray(prot.getRecievedSequenceNumber());

					if (prot.checkLastBit())
					{
						sendNak = true;
					}
				}
			}
		}		
		
		//MANGLER!!! at appende data til finalString

		if (sendNak)
		{
			std::string nakToSend = nak.createNAK();

			Afspilning nakAfspilning(nakToSend, samplesGlobal, sampleFreqGlobal);

			nakAfspilning.playString(nakToSend);
		}

		goto Modtager;
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