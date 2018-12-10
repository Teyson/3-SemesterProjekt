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



int toneLength = 500;
int sampleFreqGlobal = 8000;//41000
int samplesGlobal = (sampleFreqGlobal * toneLength)/1000;//16000;//44100
int protokolOpdelingGlobal = 32;

bool wasLastNakRecieved = true;
bool end = false;
bool isResend = false;

std::string finalBitString;
int framesSend = 3;

int main() {

	PacketSelection selecter(rand());
	
	/* initialize random seed: */
	srand(time(NULL));

	for (int i = 0; i < 1000; i++)
	{
		std::cout << "============ NEXT ITERATION ============" << std::endl << std::endl;

		int length = rand() % 4;

		std::cout << "Antallet af fejl i den simulerede sending: " << length << std::endl;

		std::string NAK = "";

		if (length == 0)
		{
			NAK = "1111";

			CRC check(NAK);

			std::string crcCheck = check.crcCheck();

			NAK = NAK + crcCheck;
		}
		else if (length == 1)
		{
			NAK = "";
			for (int j = 0; j < 4; j++)
			{
				int random = rand() % 2;
				NAK.append(std::to_string(random));
			}

			CRC check(NAK);

			std::string crcCheck = check.crcCheck();

			NAK = NAK + crcCheck;
		}

		else if (length == 2)
		{
			NAK = "";
			for (int j = 0; j < 8; j++)
			{
				int random = rand() % 2;
				NAK.append(std::to_string(random));
			}

			CRC check(NAK);

			std::string crcCheck = check.crcCheck();

			NAK = NAK + crcCheck;
		}

		else if (length == 3)
		{
			NAK = "";
			for (int j = 0; j < 12; j++)
			{
				int random = rand() % 2;
				NAK.append(std::to_string(random));
			}

			CRC check(NAK);

			std::string crcCheck = check.crcCheck();

			NAK = NAK + crcCheck;
		}
		else
		{
			std::cout << "Noget gik sgu galt.." << std::endl;
		}

		std::cout << "Det autogenerede NAK er: " << NAK << std::endl << std::endl;

		Protokol prot(NAK);

		std::vector<std::string> NAKvec = prot.getNAKs();

		std::vector<int> NAKvecint = selecter.selectPackets(NAKvec);

		std::cout << "De pakker der skal gensendes er: " << std::endl;

		for (int j = 0; j < NAKvec.size(); j++)
		{
			if (NAKvecint[j] == 15)
			{
				std::cout << "Tomt NAK! Ingen pakker gensendes!" << std::endl;
			}
			else
			{
				std::cout << NAKvecint[j] << std::endl;
			}
		}

		std::cout << std::endl;
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