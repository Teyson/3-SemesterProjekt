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
label:
	std::cout << "Afsender eller Modtager? (a/m): " << std::endl;
	std::cin >> answer;
	if (answer == '1') {						// TekstProcessing

		std::cout << "Indtast noget mere: " << std::endl;
		std::cin.clear();
		std::string firstString; std::string secondString;
		std::cin >> firstString;
		std::getline(std::cin, secondString);
		std::string input = firstString + secondString;
		
		TextProcessing tekst(input);

	}
	else if (answer == '2') {	// ToProtokol
		
	}
	else if (answer == '3') { // ToDTMF

	}
	else if (answer == '4') { // Play + Sync

	}
	else if (answer == '5') { // NAK

	}
	else {
		char answer;
		goto label;
		
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