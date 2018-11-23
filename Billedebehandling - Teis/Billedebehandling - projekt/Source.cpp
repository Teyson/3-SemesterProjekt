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

#include "Afspilning.h"
//#include "Protokol.h"
//#include "BitDTMF.h"
//#include "DTMFToner.h"


std::vector<Protokol> protokoller;

std::vector<DTMFToner> dtmfToner;
//Husk at s�tte samplerate og antal samples inde i klassen

std::vector<float> sampleToner;

int sampelsGlobal = (8000 * 20)/1000;//16000;//44100
int sampelFreqGlobal = 8000;//41000
int protokolOpdelingGlobal = 32;
int main() {
	char answer;
label:
	std::cout << "Afsender eller Modtager? (a/m): " << std::endl;
	 std::cin >> answer;

	if (answer == 'a') {						// Afsender
		Afspilning test("01010101010101010101",sampelsGlobal,sampelFreqGlobal, 0,5);


		




		




		sf::SoundBuffer Buffer;
		if (!Buffer.loadFromSamples(test.startplaying(), test.getarraySize(), 1, sampelFreqGlobal)) {
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