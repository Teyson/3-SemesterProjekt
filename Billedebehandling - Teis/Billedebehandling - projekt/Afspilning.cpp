#include "Afspilning.h"



Afspilning::Afspilning()
{
}

Afspilning::Afspilning(std::string datainput, int sampleInput, int sampleRateInput)
{
	samples = sampleInput;
	sampleFreq = sampleRateInput;
	BitDTMF sekven(datainput, samples, sampleFreq, 40);
	sekvens = sekven;
	sekvens.toProtokol(datapakker);
}

sf::Int16 * Afspilning::playThis(std::vector<int> resendPackage)
{
	int abc = 0;

	abc = makeSyncSequence(12);
	/////	Send begin	///////
	for (int i = 0; i < resendPackage.size(); i++) {
		// Opdater datapakke ved datapakker[resendPackage[i]].FUNKTION(); 
		abc = adddatapakke(resendPackage[i], abc);
	}
	/////	Send end	///////
	return raw1;
}

sf::Int16 * Afspilning::playString(std::string nakString)
{
	int bitPrTone = 4;
	int j = 0;
	int abc = 0;
	std::vector<float> tone;
	std::string acc;

	abc = makeSyncSequence(12);
	j = abc * samples;
	////	Play string	///////

	for (int i = 0; i < nakString.length() / bitPrTone; i++) {
		acc = nakString.substr(i*bitPrTone, bitPrTone);
		DTMFToner nakToner(acc);
		nakToner.setToneNumber();
		dtmfToner.push_back(nakToner);
	}
	for (int i = abc; i < dtmfToner.size(); i++) {
		tone = dtmfToner[i].createTone(samples, sampleFreq);
		for (int k = 0; k < samples; j++, k++) {
			raw1[j] = tone[k];
		}
	}
	return raw1;
}

sf::Int16* Afspilning::playSequence(int start, int antal)
{
	int j = 0;
	int abc = 0;
	std::vector<float> tone;

	abc = makeSyncSequence(12);
	j = abc * samples;

	/////	Send begin	///////
	sekvens.toDTMF(datapakker, dtmfToner, start, antal);
	std::vector<float> toner;

	for (int dtmftoneNR = abc; dtmftoneNR < dtmfToner.size(); dtmftoneNR++) {
		toner = dtmfToner[dtmftoneNR].createTone(samples, sampleFreq);
		for (int t = 0; t < samples; t++, j++) {
			raw1[j] = toner[t];
		}
	}
	/////	Send end	///////

	// Får en pinter til en vector med protokol elementer (datapakker). Her skal funktionen afspille alle pakkerne mellem start og slut, inkl. grænserne.
	//Skal på sigt implementere set LastPackageBit. 
	return raw1;
}

int Afspilning::adddatapakke(int pakke, int abc)
{
	/////	Send begin	///////
	sekvens.toDTMF(datapakker, dtmfToner, pakke, 1);
	std::vector<float> toner;
	int j = abc * samples;

	for (int dtmftoneNR = abc; dtmftoneNR < dtmfToner.size(); dtmftoneNR++) {
		toner = dtmfToner[dtmftoneNR].createTone(samples, sampleFreq);
		for (int t = 0; t < samples; t++, j++) {
			raw1[j] = toner[t];
		}
		//abc++;
	}
	return abc;
	/////	Send end	///////

}

int Afspilning::makeSyncSequence(int perioder)
{
	int abc = 0, j = 0;
	std::vector<float> tone;
	/////	sync begin	///////
	std::string sync;
	for (int i = 0; i < perioder; i++) {
		sync.append("00001111");
	}
	sync.append("0000");								// Afslutende sync tone

	std::string acc;
	for (int i = 0; i < 2 * perioder + 1; i++) {
		acc = sync.substr(i * 4, 4);
		DTMFToner syncToner(acc);
		syncToner.setToneNumber();
		dtmfToner.push_back(syncToner);
	}
	for (int i = 0; i < 2 * perioder + 1; i++) {
		tone = dtmfToner[i].createTone(samples, sampleFreq);				// Vector af floats der forløbende beskriver DTMFtonens amplitude
		for (int k = 0; k < samples; j++, k++) {
			raw1[j] = tone[k];
		}
		//abc++;
	}
	/////	sync end	///////
	return abc;
}

void Afspilning::clearRaw1DTMF()
{
	std::fill_n(raw1, arraySize, 0);
	dtmfToner.clear();
}

unsigned int Afspilning::getarraySize()
{
	return arraySize;
}

std::vector<Framing> Afspilning::getDatapakkerArray()
{
	return datapakker;
}


Afspilning::~Afspilning()
{
}
