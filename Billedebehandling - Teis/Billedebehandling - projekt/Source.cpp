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
	char answer;
label:
	std::cout << "Afsender eller Modtager? (a/m): " << std::endl;
	std::cin >> answer;
	if (answer == 'a') {						// Afsender

		std::cout << "Indtast noget mere: " << std::endl;
		std::cin.clear();
		std::string firstString; std::string secondString;
		std::cin >> firstString;
		std::getline(std::cin, secondString);
		std::string input = firstString + secondString;
		
		TextProcessing tekst(input);

		std::string bitstring = tekst.stringToBitsString();

		Afspilning afspiller(bitstring, samplesGlobal, sampleFreqGlobal);

		PacketSelection selecter(afspiller.getAntalDataPakker());

        bool nakTabt = false; 
        bool sidsteSending = false;

		int index = selecter.getPacketToSendIndex();		//k�res for at initialisere v�rdien

        afspiller.playSequence(index, framesSend);
		sf::SoundBuffer Buffer;
		Buffer.loadFromSamples(afspiller.playSequence(index, framesSend), afspiller.getAntalElementerIArray(), 1, sampleFreqGlobal);

		sf::Sound Sound;
		Sound.setBuffer(Buffer);
		Sound.play();
		while (Sound.getStatus() != 0) {
		}

		index = selecter.getPacketToSendIndex();

		//work.join();
	Afspiller:
		//Variable
		std::vector<std::string> nAKS;
		std::vector<int> nakINT;
		std::string modtagetNAKS;



		CustomRecorder recorderAfsender;
		recorderAfsender.start(8000);
		std::cout << "Lytter for NAK's...." << std::endl;
		modtagetNAKS = recorderAfsender.startThread();
		recorderAfsender.stop();	
        std::cout << "Det modtagede NAK er: " << modtagetNAKS << std::endl;
		/*
		NAK testNak;
		testNak.insertIntoArray("0000");
		testNak.insertIntoArray("0001");
		testNak.insertIntoArray("0010");
		modtagetNAKS = testNak.createNAK();
		*/
        if (modtagetNAKS.length() < 9) {
            modtagetNAKS = "0000000000000000000100000";     //Hvis den modtagede bitstreng er for kort ift. CRC-check - laves en tilf�ldig string
        }

		Framing modtagetNAKFrame(modtagetNAKS);
        sf::sleep(sf::seconds(4));
        
        if (modtagetNAKFrame.checkNAKChecksum())
        {
			std::cout << "checksum er korrekt" <<std::endl;
            if(modtagetNAKFrame.getNAKs()[0] == "1111" && sidsteSending)
            {
                goto Ending;
            }
            else if (modtagetNAKFrame.getNAKs()[0] == "1111")
            { // S�tter naktabt false og sender 3 n�ste
				std::cout << "modtaget nak 1111 (l. 114)" << std::endl;
                nakTabt = false;
                int index = selecter.getPacketToSendIndex(); //Opdater index, som returnerer n�ste pakke der skal sendes

                if (index - 1 >= afspiller.getAntalDataPakker() - framesSend) //I tilf�lde af, at det er de sidste pakker der sendes
                {
                    framesSend = afspiller.getAntalDataPakker() - index - 1;		
                    sidsteSending = true;
                }

                afspiller.playSequence(index, framesSend);		// Opdaterer getarraySize();
                Buffer.loadFromSamples(afspiller.playSequence(index, framesSend), afspiller.getAntalElementerIArray(), 1, sampleFreqGlobal);
                Sound.setBuffer(Buffer);
                Sound.play();
                while (Sound.getStatus() != 0) {
                }
            }
            else //Sender Nakpakker
            {
                nAKS = modtagetNAKFrame.getNAKs(); //Kigger p� hvilke frames der ikke er modtaget korrekt
                nakINT = selecter.selectPackets(nAKS); //Udv�lger hvilke frames der skal gensendes ud fra NAKs
                afspiller.playThis(nakINT); //Frames'ne afspilles
                Buffer.loadFromSamples(afspiller.playThis(nakINT), afspiller.getAntalElementerIArray(), 1, sampleFreqGlobal);
                Sound.setBuffer(Buffer);
                Sound.play();
                while (Sound.getStatus() != 0) {
                }
            }


        }
        else //False Checksum
        {
			std::cout << "checksum er forkert" << std::endl;
            if (nakTabt) //2 tabte nak i streg, derfor send forrige sending   -   Vi ved ikke om pakken n�r frem, derfor �ndres nakTabt ikke
            {
                afspiller.playForrigePakker();
                Buffer.loadFromSamples(afspiller.playForrigePakker(), afspiller.getAntalElementerIArray(), 1, sampleFreqGlobal);
                Sound.setBuffer(Buffer);
                Sound.play();
                while (Sound.getStatus() != 0) {
                }
                
               
            }
            else //Sende de 3 n�ste pakker
            {
                nakTabt = true;
                int index = selecter.getPacketToSendIndex(); //Opdater index, som returnerer n�ste pakke der skal sendes

                if (index - 1 >= afspiller.getAntalDataPakker() - framesSend) //I tilf�lde af, at det er de sidste pakker der sendes
                {
                    framesSend = afspiller.getAntalDataPakker() - index - 1;
                    sidsteSending = true;
                }

                afspiller.playSequence(index, framesSend);		// Opdaterer getarraySize();
                Buffer.loadFromSamples(afspiller.playSequence(index, framesSend), afspiller.getAntalElementerIArray(), 1, sampleFreqGlobal);
                Sound.setBuffer(Buffer);
                Sound.play();
                while (Sound.getStatus() != 0) {
                }

            }
        }
				
	    if (index - 1 < afspiller.getAntalDataPakker() - framesSend) {
			//std::cout << "forventet 3" << std::endl;

			goto Afspiller;
		}
		

	}
	else if (answer == 'm') {	// Modtager
		NAK nak;
		

	Modtager:
		//Variable
        CustomRecorder recorder;
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

		std::vector<Framing> modtagetFrame;

		
			for (int i = 1; i < antalOpdelinger + 1; i++)
			{
				std::string modtagetString = in.opdel(antalOpdelinger)[i];
                    
				Framing frame(modtagetString);
				modtagetFrame.push_back(frame);
                std::cout << modtagetString.size() << std::endl;
			}
            
			for (int i = 0; i < modtagetFrame.size(); i++)
			{
				Framing frame = modtagetFrame[i];
				if (frame.checkChecksum())
				{
                    std::cout << "checksummen er korrekt" << std::endl;
					nak.insertIntoArray(frame.getRecievedSequenceNumber(), frame.getRecievedData());

                    if (frame.checkResendBit())
                        isResend = true;

					//if (nak.getPointerExpected() == nak.getPointerNotRecieved() && frame.checkLastBit())  //Lastpackage????
					//{
					//	lastPackage = true;
					//}
				}
			}
		
            sf::sleep(sf::milliseconds(1000));
		
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
			std::cout << "Arraysize er: " << nakAfspilning.getarraySize() << std::endl;

			sf::SoundBuffer Buffer;
			Buffer.loadFromSamples(nakAfspilning.playString(nakToSend), nakAfspilning.getAntalElementerIArray(), 1, sampleFreqGlobal);

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

		std::cout << "Den modtagede besked er: " << modtagetBesked << std::endl;
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