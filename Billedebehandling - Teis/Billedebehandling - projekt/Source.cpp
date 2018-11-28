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
		afspiller.playSequence(selecter.getPacketToSendIndex(), framesSend);
		sf::SoundBuffer Buffer;
		Buffer.loadFromSamples(afspiller.playSequence(selecter.getPacketToSendIndex(), framesSend), afspiller.getarraySize(), 1, sampleFreqGlobal);

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
		/*recorderAfsender.start(8000);
		std::cout << "Lytter for NAK's...." << std::endl;
		modtagetNAKS = recorderAfsender.startThread();
		recorderAfsender.stop();*/

		NAK testNak;
		testNak.insertIntoArray("0000");
		testNak.insertIntoArray("0001");
		testNak.insertIntoArray("0010");
		modtagetNAKS = testNak.createNAK();

		Protokol modtagetNAKFrame(modtagetNAKS);
		

		if (modtagetNAKFrame.checkNAKChecksum()) {
			wasLastNakRecieved = true;
			if (modtagetNAKFrame.getNAKs()[0] == "1111") {		// Hvis vi modtager NAK-ingenting spilles de tre næste frames
				afspiller.playSequence(selecter.getPacketToSendIndex(), framesSend);		// Opdaterer getarraySize();
				Buffer.loadFromSamples(afspiller.playSequence(selecter.getPacketToSendIndex(), framesSend), afspiller.getarraySize(), 1, sampleFreqGlobal);
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
			if (wasLastNakRecieved==false)
			{
				wasLastNakRecieved = true;
				afspiller.playSequence(selecter.getResendIndex(),framesSend); //Frames'ne afspilles
				Buffer.loadFromSamples(afspiller.playSequence(selecter.getResendIndex(),framesSend), afspiller.getarraySize(), 1, sampleFreqGlobal);
				Sound.setBuffer(Buffer);
				Sound.play();
				while (Sound.getStatus() != 0) {
				}
				// sender 3 forrige
			}
			else {
				wasLastNakRecieved = false;
				afspiller.playSequence(selecter.getPacketToSendIndex(), framesSend); //Frames'ne afspilles
				Buffer.loadFromSamples(afspiller.playSequence(selecter.getPacketToSendIndex(), framesSend), afspiller.getarraySize(), 1, sampleFreqGlobal);
				Sound.setBuffer(Buffer);
				Sound.play();
				while (Sound.getStatus() != 0) {
				}
				// sender 3 nye
			}
		}
		
		if (!selecter.getPacketToSendIndex() - 1 < afspiller.getAntalDataPakker() - framesSend) {
			goto Afspiller;
		}
		

	}
	else if (answer == 'm') {	// Modtager
	Modtager:

		NAK nak;
		customRecorder recorder;

		recorder.start(8000);					//Start recording
		std::cout << "Recording...." << std::endl;
		std::string modtaget =  recorder.startThread();
		recorder.stop();

		bool sendNak = false;
		
		Opdeler in(modtaget);

		int antalBitPrFrame = 56;
		int antalOpdelinger = std::stoi(in.opdel(antalBitPrFrame)[0]);

		std::vector<Protokol> modtagetProt;

		//flyttes eventuelt til Protokol
		if (antalOpdelinger > 0)
		{
			for (int i = 1; i < antalOpdelinger + 1; i++)
			{
				std::string modtagetString = in.opdel(antalOpdelinger)[i];

				Protokol prot1(modtagetString);
				modtagetProt.push_back(prot1);
			}

			for (int i = 0; i < modtagetProt.size(); i++)
			{
				Protokol prot = modtagetProt[i];
				if (prot.checkChecksum())
				{
					nak.insertIntoArray(prot.getRecievedSequenceNumber());

					if (prot.checkLastBit())
					{
						sendNak = true;
					}
				}
			}
		}		
		
		//MANGLER!!! at appende data til finalString

		if (sendNak)
		{
			std::string nakToSend = nak.createNAK();

			Afspilning nakAfspilning(nakToSend, samplesGlobal, sampleFreqGlobal);

			nakAfspilning.playString(nakToSend);
		}

		goto Modtager;
	}
	else {
		char answer;
		goto label;
		
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