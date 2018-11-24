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


std::vector<Protokol> protokoller;

std::vector<DTMFToner> dtmfToner;
//Husk at s�tte samplerate og antal samples inde i klassen

std::vector<float> sampleToner;

int main() 
{
	PacketSelection PacketSelect;

	std::vector<std::string> NAKvector;
	
	std::cout << "Første sæt af 3 frames sendes" << std::endl;

	std::vector<int> selection = PacketSelect.selectPackets(NAKvector);

	PacketSelect.print(selection);

	std::cout << "Fejl på første frame" << std::endl;

	NAKvector.push_back("0001");

	selection = PacketSelect.selectPackets(NAKvector);

	PacketSelect.print(selection);

	int c;
	std::cin >> c;

	return 0;
}







//sf::SoundBuffer Buffer;
//if (!Buffer.loadFromSamples(raw, SAMPLES, 1, SAMPLE_RATE)) {
//	std::cerr << "Loading failed!" << std::endl;
//	return 1;
//}