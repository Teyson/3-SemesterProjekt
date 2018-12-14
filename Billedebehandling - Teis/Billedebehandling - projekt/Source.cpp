#include <SFML/Audio.hpp>
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <bitset>
#include <conio.h>  //kbhit
#include <thread>
#include <atomic>

#include "CImg.h"
#include "PictureProcessing.h"
//#include "customRecorder.h"

#include "Afspilning.h"
#include "Timer.h"
//#include "Protokol.h"
//#include "BitDTMF.h"
//#include "DTMFToner.h"


int sampelsGlobal = (8000 * 20)/1000;//16000;//44100
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


int main() {
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

		std::cout << "======= AFSENDERSIDE: TEST AF PLAYSTRING() =======" << std::endl << std::endl;

		std::vector<Protokol> frames = test.getDatapakkerArray();

		std::cout << "String der sendes: " << "1010101011100100111100100100010011110010" << std::endl;

		sf::SoundBuffer Buffer;
		if (!Buffer.loadFromSamples(test.playString("1010101011100100111100100100010011110010"), test.getarraySize(), 1, sampelFreqGlobal)) {
			std::cerr << "Loading failed!" << std::endl;
			return 1;
		}
		
		sf::Sound Sound;
		Sound.setBuffer(Buffer);
		Sound.play();
		while (1) {
			sf::sleep(sf::milliseconds(100));
		}
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


	///*for (int i = 0; i < recorder.getVectorSize(); i++)*/
	//{
	//	std::cout << recorder.getVector(i) << std::endl;
	//}*/

	//
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

		
	


	int c;
	std::cin >> c;

	return 0;
}







//sf::SoundBuffer Buffer;
//if (!Buffer.loadFromSamples(raw, SAMPLES, 1, SAMPLE_RATE)) {
//	std::cerr << "Loading failed!" << std::endl;
//	return 1;
//}