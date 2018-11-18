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


std::vector<Protokol> protokoller;

std::vector<DTMFToner> dtmfToner;
//Husk at s�tte samplerate og antal samples inde i klassen

std::vector<float> sampleToner;

std::vector<std::string> NAKvector;

int main()
{
	std::string data = "1001011010101001001001001101011101010101";

	Protokol ProtToSend(data, 0);

	ProtToSend.packing();

	ProtToSend.setLastBit();

	Protokol ProtRecieved(ProtToSend.getString());

	ProtRecieved.unpacking();

	std::cout << "Den inputtede streng var: " << data << std::endl;
	std::cout << "Dataen som taget af protokollen: " << ProtToSend.getData() << std::endl;
	std::cout << "CRC checksummen som taget af protokollen er: " << ProtToSend.getCRCcheck() << std::endl;
	std::cout << "Last bittet som taget af protokollen: " << ProtToSend.getLastBit() << std::endl;
	std::cout << "Sekvensnummeret som taget af protokollen er: " << ProtToSend.getSequenceNumber() << std::endl;
	std::cout << std::endl << "Den fulde protokol: " << ProtToSend.getString() << std::endl;
	std::cout << "Længden af protokollen er: " << ProtToSend.getString().length() << std::endl;

	

	std::cout << std::endl << "Når bittet er sat: " << ProtToSend.getLastBit() << std::endl;

	std::cout << std::endl << std::endl << "Den modtagede besked" << std::endl;
	std::cout << "Dataen som taget af den modtagede protokol: " << ProtRecieved.getRecievedData() << std::endl;
	std::cout << "CRC checksummen som taget af protokollen er: " << ProtRecieved.getRecievedChecksum() << std::endl;
	std::cout << std::endl <<"Når CRC-checket udføres: " << ProtRecieved.checkChecksum() << std::endl;
	std::cout << "Summen: " << ProtRecieved.writeChecksum() << std::endl;
	std::cout << "Last bittet: " << ProtRecieved.getRecievedLastBit() << std::endl;
	std::cout << "om last bittet er sat eller ej: " << ProtRecieved.checkLastBit() << std::endl;
	std::cout << "Sekvensnummeret: " << ProtRecieved.getRecievedSequenceNumber() << std::endl;
	std::cout << std::endl << "Den fulde protokol er: " << ProtRecieved.getRecievedPacket();

	int c;
	std::cin >> c;

	return 0;
}







//sf::SoundBuffer Buffer;
//if (!Buffer.loadFromSamples(raw, SAMPLES, 1, SAMPLE_RATE)) {
//	std::cerr << "Loading failed!" << std::endl;
//	return 1;
//}