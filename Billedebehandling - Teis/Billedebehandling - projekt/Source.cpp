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

#include "Afspilning.h"
#include "Timer.h"


/*
std::vector<Protokol> protokoller;

int sampelsGlobal = (8000 * 1000)/1000;//16000;//44100
int sampelFreqGlobal = 8000;//41000
int protokolOpdelingGlobal = 32;


////////// Timer //////////
bool torbenTester = true;
std::chrono::system_clock::time_point start;

void countdown() {
	
	while (torbenTester) {
		std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
		std::chrono::duration<double> differens = end - start;
		std::cout << differens.count() << std::endl;
		if (differens.count() > 1.5) {
			torbenTester = false;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}
//////////  Timer end //////////////
*/

int main() {

	NAK nak;

	std::cout << "hej" << std::endl;

	std::string abe1 = "1111100011001011001100011011011010101111";
	std::string abe2 = "1010100011001011001100011011011010100000";
	std::string abe3 = "0000100011001011001100011011011010100000";


	std::cout << "1. besked: " << abe1 << std::endl;
	std::cout << "2. besked: " << abe2 << std::endl;
	std::cout << "3. besked: " << abe3 << std::endl << std::endl;

	std::cout << "laengden af 1. besked: " << abe1.length() << std::endl;
	std::cout << "laengden af 2. besked: " << abe2.length() << std::endl;
	std::cout << "laengden af 3. besked: " << abe3.length() << std::endl << std::endl;

	Protokol prot1(abe1, 0);
	Protokol prot2(abe2, 1);
	Protokol prot3(abe3, 2);

	std::string hej = prot3.getString();

	Protokol prot1M(prot1.getString());
	Protokol prot2M(prot2.getString());
	Protokol prot3M(prot3.getString());

	prot1M.unpacking();
	prot2M.unpacking();
	prot3M.unpacking();

	std::cout << "laengden af 1. modtagede besked: " << prot1M.getRecievedData().length() << std::endl;
	std::cout << "laengden af 2. modtagede besked: " << prot2M.getRecievedData().length() << std::endl;
	std::cout << "laengden af 3. modtagede besked: " << prot3M.getRecievedData().length() << std::endl << std::endl;

	nak.insertIntoArray(prot1M.getRecievedSequenceNumber(), prot1M.getRecievedData());

	nak.insertIntoArray(prot2M.getRecievedSequenceNumber(), prot2M.getRecievedData());
	

	nak.insertIntoArray(prot3M.getRecievedSequenceNumber(), prot3M.getRecievedData());
	

	nak.createNAK();

	std::cout << "Den Samlede besked er: " << nak.getDataModtaget() << std::endl;
	std::cout << "Den samlede længde af det modtagede er: " << nak.getDataModtaget().length() << std::endl;

	
	/*
	char answer;
label:
	std::cout << "Afsender eller Modtager? (a/m): " << std::endl;
	std::cin >> answer;
	if (answer == 'a') {						// Afsender
		Afspilning test("0110110010101001",sampelsGlobal,sampelFreqGlobal);
					//	0110 1100 1010 1001
		

		std::vector<int>bla;
		bla.push_back(0);
		bla.push_back(2);
		bla.push_back(0);

		std::cout << bla.size() << std::endl;

		start = std::chrono::system_clock::now();
		std::thread work(countdown);
		sf::sleep(sf::milliseconds(1600));
		std::cout << torbenTester << std::endl;
		work.join();

		sf::SoundBuffer Buffer;
		if (!Buffer.loadFromSamples(test.playThis(bla), test.getarraySize(), 1, sampelFreqGlobal)) {
			std::cerr << "Loading failed!" << std::endl;
			return 1;
		}
		
		sf::Sound Sound;
		Sound.setBuffer(Buffer);
		Sound.play();
		while (1) {
			sf::sleep(sf::milliseconds(100));
		}
		work.join();
	}
	else if (answer == 'm') {					// Modtager
		//Custom recorder
	//if (!customRecorder::isAvailable())
	//{
	//	std::cout << "Audio capture not available";
	//	return 0;
	//}

	//customRecorder recorder;

	//recorder.start(8000);					//Start recording
	//std::cout << "Recording...." << std::endl;

	//while (!_kbhit())
	//{
	//	std::cout << recorder.getVectorSize() << std::endl;
	//}

	//recorder.stop();						//Stop recording
	//std::cout << "end recording" << std::endl;


	//for (int i = 0; i < recorder.getVectorSize(); i++)
	{
		std::cout << recorder.getVector(i) << std::endl;
	}

	
	}

	//Overwriter raw igen og igen og tildeler 
	//Laver en stor array pr. protokol objekt eller pr 
	//Predefineret antal protokoller
	//Så skal der sendes et ack hver efter et forudbestemt antal prot
	//Så behøves kun 2 arrays til at indeholde tonedata



	else {
		char answer;
		goto label;
		
	}

		
	
	*/

	return 0;
}







//sf::SoundBuffer Buffer;
//if (!Buffer.loadFromSamples(raw, SAMPLES, 1, SAMPLE_RATE)) {
//	std::cerr << "Loading failed!" << std::endl;
//	return 1;
//}