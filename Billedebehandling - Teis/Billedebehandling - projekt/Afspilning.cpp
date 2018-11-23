#include "Afspilning.h"



Afspilning::Afspilning()
{
}

Afspilning::Afspilning(std::string datainput, int sampleInput, int sampleRateInput, int start, int antal)
{
	samples = sampleInput;
	sampleFreq = sampleRateInput;
	BitDTMF sekvens(datainput, samples, sampleFreq, 4);
	sekvens.toProtokol(datapakker);

	int j = 0;
	int abc = 0;
	std::vector<float> tone;

	
	/////	sync begin	///////
	std::string sync;
	int syncperioder = 10;
	for (int i = 0; i < syncperioder; i++) {
		sync.append("00001111");
	}
	sync.append("1001");								// Afslutende sync tone
		
	std::string acc;
	for (int i = 0; i < 2 * syncperioder + 1; i++) {
		acc = sync.substr(i * 4, 4);
		DTMFToner syncToner(acc);
		syncToner.setToneNumber();
		dtmfToner.push_back(syncToner);
	}
	for (int i = 0; i < 2 * syncperioder + 1; i++) {
		tone = dtmfToner[i].createTone(samples, sampleFreq);				// Vector af floats der forløbende beskriver DTMFtonens amplitude
		for (int k = 0; k < samples; j++, k++) {
			raw1[j] = tone[k];
		}
		abc++;
	}
	/////	sync end	///////*/
	
	/////	Send begin	///////

	sekvens.toDTMF(datapakker, dtmfToner, start, antal);
	std::vector<float> toner;

	for (int dtmftoneNR = abc; dtmftoneNR < dtmfToner.size(); dtmftoneNR++) {
		toner = dtmfToner[dtmftoneNR].createTone(samples, sampleFreq);
		for (int t = 0; t < samples; t++, j++) {
			raw1[j] = toner[t];
		}
	}
	std::cout << "Hey!" << std::endl;
	/////	Send end	///////

	// Får en pinter til en vector med protokol elementer (datapakker). Her skal funktionen afspille alle pakkerne mellem start og slut, inkl. grænserne.
	//Skal på sigt implementere set LastPackageBit. 
}

sf::Int16* Afspilning::startplaying()
{
	return raw1;
}

unsigned int Afspilning::getarraySize()
{
	return arraySize;
}


Afspilning::~Afspilning()
{
}
