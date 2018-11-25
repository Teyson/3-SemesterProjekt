#include "Afspilning.h"



Afspilning::Afspilning()
{
}

Afspilning::Afspilning(std::string datainput, int sampleInput, int sampleRateInput)
{
	raw1 = new sf::Int16[arraySize];
	samples = sampleInput;
	sampleFreq = sampleRateInput;
	BitDTMF sekven(datainput, samples, sampleFreq, 40);
	sekvens = sekven;
	sekvens.toProtokol(datapakker);
}

sf::Int16 * Afspilning::playThis(std::vector<int> resendPackage)
{
	clearRaw1DTMF();
	int abc = 0;

	abc = makeSyncSequence(0);
	/////	Send begin	///////
	for (int i = 0; i < resendPackage.size(); i++) {
		datapakker[resendPackage[i]].setResendBit();		// Sætter RPB'er (Resend Package Bit) for hver pakke der bliver gensendt. 
		abc = adddatapakke(resendPackage[i], abc);
	}
	/////	Send end	///////
	return raw1;
}

sf::Int16 * Afspilning::playString(std::string nakString)
{
	clearRaw1DTMF();
	int bitPrTone = 4;
	int j = 0;
	int abc = 0;
	std::vector<float> tone;
	std::string acc;

	abc = makeSyncSequence(0);
	j = abc * samples;													// j er elementer i raw1
		////	Play string	///////
	for (int i = 0; i < nakString.length() / bitPrTone; i++) {
		acc = nakString.substr(i*bitPrTone, bitPrTone);					// Tager en del af stringen og laver den til en tone. 
		DTMFToner nakToner(acc);
		nakToner.setToneNumber();
		dtmfToner.push_back(nakToner);
	}
	for (int i = abc; i < dtmfToner.size(); i++) {
		tone = dtmfToner[i].createTone(samples, sampleFreq);			// Tager et toneNr og konvertere det til en vector med amplituder
		for (int k = 0; k < samples; j++, k++) {
			raw1[j] = tone[k];
		}
	}
	return raw1;
}

sf::Int16* Afspilning::playSequence(int start, int antal)
{
	clearRaw1DTMF();
	int j = 0;
	int abc = 0;
	std::vector<float> tone;

	abc = makeSyncSequence(1);
	j = abc * samples;

	/////	Send begin	///////
	sekvens.toDTMF(datapakker, dtmfToner, start, antal);								// Tager datapakker fra sekvensen startende fra start og giver det et toneNr. Dette gør den 'antal' gange
	std::vector<float> toner;

	for (int dtmftoneNR = abc; dtmftoneNR < dtmfToner.size(); dtmftoneNR++) {
		toner = dtmfToner[dtmftoneNR].createTone(samples, sampleFreq);					// Tager et toneNr og konvertere det til en vector med amplituder
		for (int t = 0; t < samples; t++, j++) {
			raw1[j] = toner[t];															// Tilføjer amplitudeværdierne i afspilningsarrayet.
		}
	}
	/////	Send end	///////

	// Får en pinter til en vector med protokol elementer (datapakker). Her skal funktionen afspille alle pakkerne mellem start og slut, inkl. grænserne.
	//Skal på sigt implementere set LastPackageBit. 
	return raw1;
}

int Afspilning::adddatapakke(int pakke,int abc)
{
	/////	Send begin	///////
	sekvens.toDTMF(datapakker, dtmfToner, pakke, 1);									// Tager datapakker fra sekvensen startende fra start og giver det et toneNr. Dette gør den 1 gang
	std::vector<float> toner;
	int j = abc * samples;

	for (int dtmftoneNR = abc; dtmftoneNR < dtmfToner.size(); dtmftoneNR++) {
		toner = dtmfToner[dtmftoneNR].createTone(samples, sampleFreq);					// Tager et toneNr og konvertere det til en vector med amplituder
		for (int t = 0; t < samples; t++, j++) {
			raw1[j] = toner[t];															// Tilføjer amplitudeværdierne i afspilningsarrayet.
		}
		abc++;
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
	//sync.append("1001");								// Afslutende sync tone

	std::string acc;
	for (int i = 0; i < 2 * perioder; i++) {
		acc = sync.substr(i * 4, 4);
		DTMFToner syncToner(acc);
		syncToner.setToneNumber();
		dtmfToner.push_back(syncToner);
	}
	for (int i = 0; i < 2 * perioder; i++) {
		tone = dtmfToner[i].createTone(samples, sampleFreq);				// Vector af floats der forløbende beskriver DTMFtonens amplitude
		for (int k = 0; k < samples; j++, k++) {
			raw1[j] = tone[k];
		}
		abc++;
	}
	/////	sync end	///////
	return abc;									// retunere antallet af toner i afspilningsarrayet (raw1)
}

void Afspilning::clearRaw1DTMF()
{
	std::fill_n(raw1, arraySize, 0);			// tømmer og nulstiller afspillingsarrayet (raw1)
	dtmfToner.clear();							// clearer vektoren der indeholder DTMF-tonerne
}

unsigned int Afspilning::getarraySize()
{
	return arraySize;
}


Afspilning::~Afspilning()
{
}
