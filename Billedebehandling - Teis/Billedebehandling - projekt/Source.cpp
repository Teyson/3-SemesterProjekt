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

	NAK nak;
	
	/* initialize random seed: */
	srand(time(NULL));

	nak.updatePointerExpected();

	std::cout << "frame 0, 1 og 2 sendes, og alle modtages" << std::endl;

	nak.insertIntoArray("0000", "1010101010111010101010111011010010110001");
	nak.insertIntoArray("0001", "1010101111101010010010011101100101010100");
	nak.insertIntoArray("0010", "1010010110001101001010110110100100101101");

	std::string createdNak = nak.createNAK();

	std::cout << "Det NAK der bliver lavet er saa: " << createdNak << std::endl << std::endl;;
	std::cout << "Altsaa et tomt NAK. Derfor sendes frame 3, 4, 5. Frame 5 gaar tabt." << std::endl;

	nak.updatePointerExpected();

	nak.insertIntoArray("0011", "0100101000111010110011010100101001011101");
	nak.insertIntoArray("0100", "0100101001010010010110101010101011010101");
	
	createdNak = nak.createNAK();

	std::cout << "Det NAK der bliver lavet er saa: " << createdNak << std::endl << std::endl;
	std::cout << "Altsaa et NAK paa frame 5. Frame 5 gensendes og modtages." << std::endl;

	nak.insertIntoArray("0101", "0100101001001001000000000000000111111001");

	createdNak = nak.createNAK();

	std::cout << "Det NAK der bliver lavet er saa: " << createdNak << std::endl << std::endl;
	std::cout << "Altsaa et tomt NAK, derfor sendes frame 6, 7, 8. Frame 6 og 7 gaar tabt" << std::endl;

	nak.updatePointerExpected();

	nak.insertIntoArray("1000", "0111101110111111111111111101010101010001");

	createdNak = nak.createNAK();

	std::cout << "Det NAK der bliver lavet er saa: " << createdNak << std::endl << std::endl;
	std::cout << "Altsaa et NAK paa frame 6 og 7. Framesne gensendes og modtages" << std::endl;
	
	nak.insertIntoArray("0110", "0110011001110100101101101001010101010110");
	nak.insertIntoArray("0111", "0101001010001010010001100101011101010111");
	
	createdNak = nak.createNAK();

	std::cout << "Det NAK der bliver lavet er saa: " << createdNak << std::endl << std::endl;
	std::cout << "Altsaa et tom NAK." << std::endl;

	int c;
	std::cin >> c;

	return 0;
}







//sf::SoundBuffer Buffer;
//if (!Buffer.loadFromSamples(raw, SAMPLES, 1, SAMPLE_RATE)) {
//	std::cerr << "Loading failed!" << std::endl;
//	return 1;
//}