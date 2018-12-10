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
	
	for (int i = 0; i < 2; i++)
	{
		std::string input = "";

		std::cout << "Indtast venligst en tekst: ";

		std::getline(std::cin, input);

		std::cout << "Den inputtede streng er: " << input << std::endl << std::endl;

		TextProcessing textprocesser(input);

		std::string bitinput = textprocesser.stringToBitsString();

		BitDTMF bit(bitinput, samplesGlobal, sampleFreqGlobal, 40);

		std::vector<Protokol> protokolVec;

		bit.toProtokol(protokolVec);

		std::cout << "Den inputtede streng opdeles i " << protokolVec.size() << " pakker, af 40 bit hver." << std::endl << std::endl;

		std::cout << "Den foerste pakkes data bliver dermed: " << protokolVec[0].getData() << " og laengden af denne bitstreng er: " << protokolVec[0].getData().size() << std::endl << std::endl;

		std::cout << "Den sidste pakkes data bliver dermed: " << protokolVec[protokolVec.size() - 1].getData() << " og laengden af denne bitstreng er: " << protokolVec[protokolVec.size() - 1].getData().size() << std::endl << std::endl;

		PacketSelection selecter(protokolVec.size());

		int index = selecter.getPacketToSendIndex();

		std::cout << "Den naeste frame der skal sendes er: " << index << std::endl << std::endl;

		std::cout << "Eftersom der sendes " << framesSend << " frames, vil frame 0, 1 og 2 blive sendt" << std::endl << std::endl;

		index = selecter.getPacketToSendIndex();

		std::cout << "Efter denne sending bliver nextPacketIndex: " << index << "." << std::endl << std::endl;

		std::cout << "Hvis der kigges paa den foerste frame vil den faerdige frame efter framing vaere: " << protokolVec[0].getString() << ". Den er " << protokolVec[0].getString().size() << " bits lang" << std::endl << std::endl;

		std::cout << "============ NAESTE ITERATION ============" << std::endl << std::endl;

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