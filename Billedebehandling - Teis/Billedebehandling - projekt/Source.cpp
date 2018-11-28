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
#include <sstream>

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



std::vector<Framing> protokoller;

int samplesGlobal = (8000 * 20)/1000;//16000;//44100
int sampleFreqGlobal = 8000;//41000
int protokolOpdelingGlobal = 32;
int framesSend=3;

std::string finalBitString;
////////// Variabler der bliver brugt til protokol /////////

bool wasLastNakNotRecieved = false;

///////// END   /////////

////////// Timer //////////
double runOut = 1.5;
bool runnedOut;
std::chrono::system_clock::time_point start;

void countdown() {
	runnedOut = false;
	while (!runnedOut) {
		std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
		std::chrono::duration<double> differens = end - start;
		std::cout << differens.count() << std::endl;
		if (differens.count() > runOut) {
			runnedOut = true;
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
	if (answer == 'a') {	// Afsender
				
		std::string input = "Jeg plukker frugt med en brugt frugtplukker, og kommer det i en brugt plastikpose";
		//std::cout << "Skriv tekst der skal sendes: " << std::endl;
		
		/*std::getline(std::cin, input);
		std::cout << input << std::endl;*/
		TextProcessing tekst(input);

		std::string bitstring = tekst.stringToBitsString();

		Afspilning afspiller(bitstring, samplesGlobal, sampleFreqGlobal);

		PacketSelection selecter;

		

		/*sf::SoundBuffer Buffer;
		Buffer.loadFromSamples(afspiller.playSequence(selecter.getPacketToSendIndex(), framesSend), afspiller.getarraySize(), 1, sampleFreqGlobal);

		sf::Sound Sound;
		Sound.setBuffer(Buffer);
		Sound.play();
		while (Sound.getStatus() != 0) {
		}	*/							// linie 87-94 funktion for sig i Afspiller klassen. raw1 array kun stor nok til at tonerne bliver spillet
		std::cout << "fisk" << std::endl;
		
		/* // Timer
		start = std::chrono::system_clock::now();
		std::thread work(countdown);
		sf::sleep(sf::milliseconds(1600));
		std::cout << torbenTester << std::endl;
		*/
		
		//work.join();

	Afspiller:
		customRecorder recorderAfsender;
		recorderAfsender.start(8000);
		std::cout << "Lytter for NAK's...." << std::endl;
		std::string modtagetNAKS = recorderAfsender.startThread();
		recorderAfsender.stop();
		Framing modtagetNAKFrame(modtagetNAKS);
		std::vector<std::string> nAKS;
		std::vector<int> nakINT;

		if (modtagetNAKFrame.checkNAKChecksum()) {
			wasLastNakNotRecieved = false;
			if (modtagetNAKFrame.getNAKs()[0] == "1111") {		// Hvis vi modtager NAK-ingenting
				afspiller.playSequence(selecter.getPacketToSendIndex(), framesSend);
			}
			nAKS = modtagetNAKFrame.getNAKs();
			nakINT = selecter.selectPackets(nAKS);
			afspiller.playThis(nakINT);
		}
		//work.join();
		else if (runnedOut) // I tilfaelde af Timeren løber ud
		{
			if (wasLastNakNotRecieved) 
			{
				// Afspiller de sidste 3
			}
			else {
				// sender 3 nye
			}
		}


		goto Afspiller;

		
	}
	else if (answer == 'm') {	// Modtager
		NAK nak;
		customRecorder recorder;

	Modtager:
		recorder.start(8000);					//Start recording
		std::cout << "Recording...." << std::endl;
		std::string modtaget =  recorder.startThread();
		recorder.stop();
        std::cout << modtaget << std::endl;
		
        float mistake = 0;
        std::string check = "0011011111110010100110000001111010110010101001011101011011000000";
        for (size_t i = 0; i < 64; i++)
        {
            if (modtaget[i] != check[i])
                mistake++;
        }
        std::cout << mistake / ((float)64) * 100 << std::endl;

        
        bool sendNak = false;
		
		Opdeler in(modtaget);

		int antalBitPrFrame = 56;
		int antalOpdelinger = std::stoi(in.opdel(antalBitPrFrame)[0]);

		std::vector<Framing> modtagetFrame;

		//flyttes eventuelt til Protokol
		if (antalOpdelinger > 0)
		{
			for (int i = 1; i < antalOpdelinger + 1; i++)
			{
				std::string modtagetString = in.opdel(antalOpdelinger)[i];

				Framing frame(modtagetString);
				modtagetFrame.push_back(frame);
			}

			for (int i = 0; i < modtagetFrame.size(); i++)
			{
				Framing frame = modtagetFrame[i];
				if (frame.checkChecksum())
				{
					nak.insertIntoArray(frame.getRecievedSequenceNumber());

					if (frame.checkLastBit())
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