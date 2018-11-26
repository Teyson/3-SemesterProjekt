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

std::string finalBitString;

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
		Afspilning test("1010101000011011111111100001101011100100",samplesGlobal,sampleFreqGlobal);
					//	0110 1100 1010 1001
	
		/*std::string str = "Jeg holder af at spise kage!";
		TextProcessing str_txt(str);
		std::string str_bits=str_txt.stringToBitsString();
		Bit2Tekst aben_b(str_bits);
		std::cout << aben_b.bitToString() << std::endl;

		Afspilning reA(str_bits, samplesGlobal, sampleFreqGlobal);*/

		

		/* // 
		start = std::chrono::system_clock::now();
		std::thread work(countdown);
		sf::sleep(sf::milliseconds(1600));
		std::cout << torbenTester << std::endl;
		*/

		sf::SoundBuffer Buffer;
		if (!Buffer.loadFromSamples(test.playSequence(0,1), test.getarraySize(), 1, sampleFreqGlobal)) {
			std::cerr << "Loading failed!" << std::endl;
			return 1;
		}
		
		sf::Sound Sound;
		Sound.setBuffer(Buffer);
		Sound.play();
		while (1) {
			sf::sleep(sf::milliseconds(100));
		}
		//work.join();
	}
	else if (answer == 'm') {	// Modtager
	Modtager:

		NAK nak;
		customRecorder recorder;

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