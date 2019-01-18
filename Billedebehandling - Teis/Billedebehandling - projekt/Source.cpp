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
int toneLength = 20;
int sampleFreqGlobal = 8000;//41000
int samplesGlobal = (sampleFreqGlobal * toneLength)/1000;//16000;//44100
int protokolOpdelingGlobal = 32;

bool wasLastNakRecieved = true;

std::string finalBitString;
int framesSend = 3;


int main() {
	char answer;
label:
	std::cout << "Afsender eller Modtager? (a/m): " << std::endl;
	std::cin >> answer;
	if (answer == 'a') {						// Afsender
		bool end = false;
		std::string input = "Hej mit navn er Teis, min ynglingshobby er at fappe meget kraftigt imens jeg ligger i min seng og ser porno.";
		//std::cout << "Skriv tekst der skal sendes: " << std::endl;

		/*std::getline(std::cin, input);
		std::cout << input << std::endl;*/
		TextProcessing tekst(input);

		std::string bitstring = tekst.stringToBitsString();

		Afspilning afspiller(bitstring, samplesGlobal, sampleFreqGlobal);

		PacketSelection selecter(afspiller.getAntalDataPakker());

		

		/* // 
		start = std::chrono::system_clock::now();
		std::thread work(countdown);
		sf::sleep(sf::milliseconds(1600));
		std::cout << torbenTester << std::endl;
		*/
		int index = 0;

		afspiller.playSequence(index, framesSend);
		sf::SoundBuffer Buffer;
		Buffer.loadFromSamples(afspiller.playSequence(index, framesSend), afspiller.getarraySize(), 1, sampleFreqGlobal);

		sf::Sound Sound;
		Sound.setBuffer(Buffer);
		Sound.play();
		while (Sound.getStatus() != 0) {
		}
		std::cout << "fisk" << std::endl;

		//work.join();
	Afspiller:
		//Variable
		std::vector<std::string> nAKS;
		std::vector<int> nakINT;
		std::string modtagetNAKS;

		customRecorder recorderAfsender;
		recorderAfsender.start(8000);
		std::cout << "Lytter for NAK's...." << std::endl;
		modtagetNAKS = recorderAfsender.startThread();
		recorderAfsender.stop();	
		
		/*
		NAK testNak;
		testNak.insertIntoArray("0000");
		testNak.insertIntoArray("0001");
		testNak.insertIntoArray("0010");
		modtagetNAKS = testNak.createNAK();
		*/

		if (modtagetNAKS.length() > 7)
		{
			modtagetNAKS = "00000000000000000010000000";
		}
		
		Protokol modtagetNAKFrame(modtagetNAKS);


		if (modtagetNAKFrame.checkNAKChecksum()) {
			wasLastNakRecieved = true;

			std::cout << "Fejl 1" << std::endl;

			if (end && modtagetNAKFrame.getNAKs()[0] == "1111")
			{
				goto Ending;
			}

			if (modtagetNAKFrame.getNAKs()[0] == "1111") {		// Hvis vi modtager NAK-ingenting spilles de tre n�ste frames
				int index = selecter.getPacketToSendIndex();
				
				if (index - 1 >= afspiller.getAntalDataPakker() - framesSend)
				{
					framesSend = afspiller.getAntalDataPakker() - index - 1;		//Bruges kun i slutningen af sendingen
					end = true;
					
				}

				afspiller.playSequence(index, framesSend);		// Opdaterer getarraySize();
				Buffer.loadFromSamples(afspiller.playSequence(index, framesSend), afspiller.getarraySize(), 1, sampleFreqGlobal);
				Sound.setBuffer(Buffer);
				Sound.play();
				while (Sound.getStatus() != 0) {
				}
			}
			else {
				nAKS = modtagetNAKFrame.getNAKs(); //Kigger p� hvilke frames der ikke er modtaget korrekt
				nakINT = selecter.selectPackets(nAKS); //Udv�lger hvilke frames der skal gensendes ud fra NAKs
				afspiller.playThis(nakINT); //Frames'ne afspilles
				Buffer.loadFromSamples(afspiller.playThis(nakINT), afspiller.getarraySize(), 1, sampleFreqGlobal);
				Sound.setBuffer(Buffer);
				Sound.play();
				while (Sound.getStatus() != 0) {
				}
			}
		}

		else
		{
			std::cout << "Forventet 1" << std::endl;

			if (end)		//KIG P� DET HER FUCKING LORT!! PROBLEMATIC AS FUCK!
			{
				//her vil vi gensende de allersidste pakker. Det kan v�re alt fra 1 til antallet af pakker vi sender.

				afspiller.playSequence(selecter.getResendIndexLast(framesSend), framesSend);		// Opdaterer getarraySize();
				Buffer.loadFromSamples(afspiller.playSequence(selecter.getResendIndexLast(framesSend), framesSend), afspiller.getarraySize(), 1, sampleFreqGlobal);
				Sound.setBuffer(Buffer);
				Sound.play();
				while (Sound.getStatus() != 0) {
				}
			}
			else if (wasLastNakRecieved==false)
			{
				afspiller.playSequence(selecter.getResendIndex(),framesSend); //Frames'ne afspilles
				Buffer.loadFromSamples(afspiller.playSequence(selecter.getResendIndex(),framesSend), afspiller.getarraySize(), 1, sampleFreqGlobal);
				Sound.setBuffer(Buffer);
				Sound.play();
				while (Sound.getStatus() != 0) {
				}
				// sender 3 forrige
			}
			else {
				int index = selecter.getPacketToSendIndex();
				std::cout << "Forventet 2" << std::endl;

				if (index - 1 >= afspiller.getAntalDataPakker() - framesSend)
				{
					framesSend = afspiller.getAntalDataPakker() - index - 1;		//Bruges kun i slutningen af sendingen
					end = true;
				}

				wasLastNakRecieved = false;
				afspiller.playSequence(index, framesSend); //Frames'ne afspilles
				Buffer.loadFromSamples(afspiller.playSequence(index, framesSend), afspiller.getarraySize(), 1, sampleFreqGlobal);
				Sound.setBuffer(Buffer);
				Sound.play();
				while (Sound.getStatus() != 0) {
				}
				// sender 3 nye
			}
		}
		
		bool test = (index - 1 < afspiller.getAntalDataPakker() - framesSend);

		std::cout << test << std::endl;

		if (index - 1 < afspiller.getAntalDataPakker() - framesSend) {
			std::cout << "forventet 3" << std::endl;

			goto Afspiller;
		}
		

	}
	else if (answer == 'm') {	// Modtager
		NAK nak;
		

	Modtager:
		customRecorder recorder;
		//Variable
		float mistake = 0;
		std::string modtaget;
		std::string check = "0011011111110010100110000001111010110010101001011101011011000000";
		bool sendNak = false;
		bool lastPackage = false;

		recorder.start(sampleFreqGlobal);					//Start recording
		std::cout << "Recording...." << std::endl;
		modtaget = recorder.startThread();
		recorder.stop();
		std::cout << "RECORDER STOPPET!!!!!!!!!" << std::endl;

		std::cout << "L�ngden p� det modtagede er: " << modtaget.length() << std::endl;
		std::cout << "Bitstrengen er: " << modtaget << std::endl;

		Opdeler in(modtaget);

		int antalBitPrFrame = 56;
		int antalOpdelinger = std::stoi(in.opdel(antalBitPrFrame)[0]);

		std::vector<Protokol> modtagetFrame;

		if (antalOpdelinger > 0)
		{
			for (int i = 1; i < antalOpdelinger + 1; i++)
			{
				std::string modtagetString = in.opdel(antalOpdelinger)[i];
                    
				Protokol frame(modtagetString);
				modtagetFrame.push_back(frame);
			}
            
			for (int i = 0; i < modtagetFrame.size(); i++)
			{
				Protokol frame = modtagetFrame[i];
				if (frame.checkChecksum())
				{
                    std::cout << "checksummen er korrekt" << std::endl;
					nak.insertIntoArray(frame.getRecievedSequenceNumber(), frame.getRecievedData());

					if (nak.getPointerExpected() == nak.getPointerNotRecieved() && frame.checkLastBit())
					{
						lastPackage = true;
					}
				}
			}
		}

		
		if (!lastPackage)
		{
			sf::sleep(sf::seconds(1));

			std::string nakToSend = nak.createNAK();

			std::cout << nakToSend << std::endl;
			//std::cout << nak.getDataModtaget() << std::endl;
			std::cout << "pointerExpected er: " << nak.getPointerExpected() << std::endl;
			std::cout << "pointerNotRecieved er: " << nak.getPointerNotRecieved() << std::endl;
			std::cout << "pointerMax er: " << nak.getPointerMax() << std::endl;

			std::string pointerNRString = std::bitset<4>(nak.getPointerNotRecieved()).to_string();

			std::cout << "dataen ved pointerNotRecieved er: " << nak.getDataFromArray(pointerNRString) << std::endl;

			Afspilning nakAfspilning(nakToSend, samplesGlobal, sampleFreqGlobal);

			nakAfspilning.playString(nakToSend);

			sf::SoundBuffer Buffer;
			Buffer.loadFromSamples(nakAfspilning.playString(nakToSend), nakAfspilning.getarraySize(), 1, sampleFreqGlobal);

			sf::Sound Sound;
			Sound.setBuffer(Buffer);
			Sound.play();
			while (Sound.getStatus() != 0) {
			}

			//Husk at lave om s� Last bit nu betyder allersidste frame
			if (modtagetFrame.size() > 0)
			{
				if (!modtagetFrame[modtagetFrame.size() - 1].checkLastBit() && !(nak.getPointerExpected() == nak.getPointerNotRecieved()))
				{
					goto Modtager;
				}
                std::cout << "Lastbit er sat:  " << modtagetFrame[modtagetFrame.size() - 1].checkLastBit() << std::endl;
			}
			else
			{
				goto Modtager;
			}
		}

		//Slutnignen for modtageren

		std::string dataModtaget = nak.getDataModtaget();

		Bit2Tekst toTekst(dataModtaget);

		std::string modtagetBesked = toTekst.bitToString();

		std::cout << "Den modtagede besked er: " << std::endl;
		std::cout << modtagetBesked << std::endl;
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