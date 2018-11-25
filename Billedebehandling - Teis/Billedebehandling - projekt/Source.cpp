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


std::vector<Protokol> protokoller;

std::vector<DTMFToner> dtmfToner;
//Husk at s�tte samplerate og antal samples inde i klassen

std::vector<float> sampleToner;


int main()
{
	NAK NAKtest;

	std::cout << "Alle pakker går igennem: " << std::endl << std::endl;
	
	NAKtest.insertIntoArray("0000");
	NAKtest.insertIntoArray("0001");
	NAKtest.insertIntoArray("0010");

	std::cout << "createNAK testes, forventet output: 1111 efterfulgt af crc sum." << std::endl << " Output: " << NAKtest.createNAK() << std::endl;

	NAKtest.reset();

	std::cout << std::endl << "Ikke alle pakker går igennem" << std::endl << std::endl;

	NAKtest.insertIntoArray("0000");
	NAKtest.insertIntoArray("0010");

	std::cout << "createNAK testes, forventet output: 0001 efterfulgt at crc sum." << std::endl << " Output: " << NAKtest.createNAK() << std::endl;

	std::cout << "pointerNotRecieved: " << NAKtest.getPointerNotRecieved() << std::endl << "pointerMax: " << NAKtest.getPointerMax() << std::endl << "pointerExpected: " << NAKtest.getPointerExpected() << std::endl;

	NAKtest.reset();

	std::cout << std::endl << "Alle pakker går igennem, efterfulgt af at en pakke går tabt" << std::endl << std::endl;

	NAKtest.insertIntoArray("0000");
	NAKtest.insertIntoArray("0001");
	NAKtest.insertIntoArray("0010");

	std::cout << NAKtest.getPointerExpected() << std::endl;

	std::cout << "createNAK testes, forventet output: 1111 efterfulgt at crc sum." << std::endl << " Output: " << NAKtest.createNAK() << std::endl;

	std::cout << "pointerNotRecieved: " << NAKtest.getPointerNotRecieved() << std::endl << "pointerMax: " << NAKtest.getPointerMax() << std::endl << "pointerExpected: " << NAKtest.getPointerExpected() << std::endl;

	NAKtest.insertIntoArray("0011");
	NAKtest.insertIntoArray("0101");

	std::cout << std::endl << "createNAK testes, forventet output: 0100 efterfulgt af crc sum." << std::endl << " Output: " << NAKtest.createNAK() << std::endl;

	std::cout << "pointerNotRecieved: " << NAKtest.getPointerNotRecieved() << std::endl << "pointerMax: " << NAKtest.getPointerMax() << std::endl << "pointerExpected: " << NAKtest.getPointerExpected() << std::endl;

	NAKtest.reset();

	std::cout << std::endl << "En pakke går galt, NAK går galt, derfor kommer de næste 3 pakker, NAK går galt, derfor kommer de samme 3 pakker igen" << std::endl << std::endl;

	NAKtest.insertIntoArray("0000");
	NAKtest.insertIntoArray("0010");

	std::cout << NAKtest.getPointerExpected() << std::endl;

	std::cout << "createNAK testes, forventet output: 0001 efterfulgt at crc sum." << std::endl << " Output: " << NAKtest.createNAK() << std::endl;

	std::cout << "pointerNotRecieved: " << NAKtest.getPointerNotRecieved() << std::endl << "pointerMax: " << NAKtest.getPointerMax() << std::endl << "pointerExpected: " << NAKtest.getPointerExpected() << std::endl;

	NAKtest.insertIntoArray("0011");
	NAKtest.insertIntoArray("0100");
	NAKtest.insertIntoArray("0101");

	std::cout << std::endl << "createNAK testes, forventet output: 0001 efterfulgt af crc sum." << std::endl << " Output: " << NAKtest.createNAK() << std::endl;

	std::cout << "pointerNotRecieved: " << NAKtest.getPointerNotRecieved() << std::endl << "pointerMax: " << NAKtest.getPointerMax() << std::endl << "pointerExpected: " << NAKtest.getPointerExpected() << std::endl;

	NAKtest.insertIntoArray("0011");
	NAKtest.insertIntoArray("0100");
	NAKtest.insertIntoArray("0101");

	std::cout << std::endl << "createNAK testes, forventet output: 0001 efterfulgt af crc sum." << std::endl << " Output: " << NAKtest.createNAK() << std::endl;

	std::cout << "pointerNotRecieved: " << NAKtest.getPointerNotRecieved() << std::endl << "pointerMax: " << NAKtest.getPointerMax() << std::endl << "pointerExpected: " << NAKtest.getPointerExpected() << std::endl;


	std::string packet1 = "1111000010101011010101101011011000001111";
	std::string packet2 = "1111000010101011010101101011011000001111";
	std::string packet3 = "1111000010101011010101101011011000001111";
	std::cout << packet1.length() << std::endl;


	/*std::string data = "1001011010101001001001001101011101010101";

	std::string data = "1001011010101001001001001101011101010101";
	
	Protokol ProtToSend(data, 0);

	ProtToSend.packing();

	ProtToSend.setLastBit();

	ProtToSend.setResendBit();

	Protokol ProtRecieved(ProtToSend.getString());

	ProtRecieved.unpacking();

	std::cout << "Den inputtede streng var: " << data << std::endl;
	std::cout << "Dataen som taget af protokollen: " << ProtToSend.getData() << std::endl;
	std::cout << "CRC checksummen som taget af protokollen er: " << ProtToSend.getCRCcheck() << std::endl;
	std::cout << "Last bittet som taget af protokollen: " << ProtToSend.getLastBit() << std::endl;
	std::cout << "Sekvensnummeret som taget af protokollen er: " << ProtToSend.getSequenceNumber() << std::endl;
	std::cout << std::endl << "Den fulde protokol: " << ProtToSend.getString() << std::endl;
	std::cout << "Længden af protokollen er: " << ProtToSend.getString().length() << std::endl;
	std::cout << "resendBit: " << ProtToSend.getResendBit() << std::endl;

	ProtToSend.clearLastBit();
	std::cout << "Lastbit efter clear: " << ProtToSend.getLastBit() << std::endl;



	/*

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
	*/
	int c;
	std::cin >> c;

	return 0;
}







//sf::SoundBuffer Buffer;
//if (!Buffer.loadFromSamples(raw, SAMPLES, 1, SAMPLE_RATE)) {
//	std::cerr << "Loading failed!" << std::endl;
//	return 1;
//}