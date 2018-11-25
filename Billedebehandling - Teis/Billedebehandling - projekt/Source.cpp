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
#include "TextProcessing.h"
#include "DTMFToner.h"
#include "NAK.h"
#include "PacketSelection.h"
#include "Bit2Tekst.h"


std::vector<Protokol> protokoller;

std::vector<DTMFToner> dtmfToner;
//Husk at s�tte samplerate og antal samples inde i klassen

std::vector<float> sampleToner;

int main() 
{
	TextProcessing processer("e");

	std::string hej = processer.stringToBitsString();
		
	std::cout << hej << std::endl;

	std::cout << std::stoi(hej, nullptr, 2) << std::endl;

	int c;
	std::cin >> c;

	return 0;
}







//sf::SoundBuffer Buffer;
//if (!Buffer.loadFromSamples(raw, SAMPLES, 1, SAMPLE_RATE)) {
//	std::cerr << "Loading failed!" << std::endl;
//	return 1;
//}