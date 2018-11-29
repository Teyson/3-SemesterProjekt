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
////////// Timer //////////
double runOut = 1.5;
bool torbenTester = true;
std::chrono::system_clock::time_point start;

void countdown() {
	
	while (torbenTester) {
		std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
		std::chrono::duration<double> differens = end - start;
		std::cout << differens.count() << std::endl;
		if (differens.count() > runOut) {
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
		bool end = false;
		std::string input = "Jeg plukker frugt med en brugt frugtplukker, og kommer det i en brugt plastikpose";
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
		int index = selecter.getPacketToSendIndex();

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
		
		Protokol modtagetNAKFrame(modtagetNAKS);
		

		if (modtagetNAKFrame.checkNAKChecksum()) {
			wasLastNakRecieved = true;

			std::cout << "Fejl 1" << std::endl;

			if (end && modtagetNAKFrame.getNAKs()[0] == "1111")
			{
				goto Ending;
			}

			if (modtagetNAKFrame.getNAKs()[0] == "1111") {		// Hvis vi modtager NAK-ingenting spilles de tre næste frames
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
				nAKS = modtagetNAKFrame.getNAKs(); //Kigger på hvilke frames der ikke er modtaget korrekt
				nakINT = selecter.selectPackets(nAKS); //Udvælger hvilke frames der skal gensendes ud fra NAKs
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

			if (end)		//KIG PÅ DET HER FUCKING LORT!! PROBLEMATIC AS FUCK!
			{
				//her vil vi gensende de allersidste pakker. Det kan være alt fra 1 til antallet af pakker vi sender.

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
		customRecorder recorder;

Modtager:
		//Variable
		float mistake = 0;
		std::string modtaget;
		std::string check = "0011011111110010100110000001111010110010101001011101011011000000";
		bool sendNak = false;

		recorder.start(sampleFreqGlobal);					//Start recording
		std::cout << "Recording...." << std::endl;
		modtaget = recorder.startThread();
		recorder.stop();

		//Check til forsøg 
		//for (size_t i = 0; i < 64; i++) 
		//{
		//    if (modtaget[i] != check[i])
		//        mistake++;
		//}
		//std::cout << mistake / ((float)64) * 100 << std::endl;

		Opdeler in(modtaget);

		int antalBitPrFrame = 56;
		int antalOpdelinger = std::stoi(in.opdel(antalBitPrFrame)[0]);

		std::vector<Protokol> modtagetFrame;

		//flyttes eventuelt til Protokol
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
					nak.insertIntoArray(frame.getRecievedSequenceNumber(), frame.getData());
				}
			}
		}

		std::string nakToSend = nak.createNAK();

		Afspilning nakAfspilning(nakToSend, samplesGlobal, sampleFreqGlobal);

		nakAfspilning.playString(nakToSend);
		
		//Husk at lave om så Last bit nu betyder allersidste frame
		if (!modtagetFrame[modtagetFrame.size() - 1].checkLastBit() && !(nak.getPointerExpected() == nak.getPointerNotRecieved()))
		{
			goto Modtager;
		}

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