#include "Afspilning.h"



Afspilning::Afspilning()
{
}

Afspilning::Afspilning(std::string datainput, int sampleInput, int sampleRateInput)
{
	raw1 = new sf::Int16[arraySize];// En tone + antal sync + buffer
	samples = sampleInput;
	sampleFreq = sampleRateInput;
	BitDTMF sekven(datainput, samples, sampleFreq, 40);
	sekvens = sekven;
	sekvens.toProtokol(datapakker);
	setLastBit();
}

sf::Int16 * Afspilning::playThis(std::vector<int> resendPackage)
{
	clearRaw1DTMF();
	int abc = 0;
    forrigeSendteDatapakker.clear();
    forrigeSendteDatapakker = resendPackage;
	
	
	abc = makeSyncSequence();
	/////	Send begin	///////
	for (int i = 0; i < resendPackage.size(); i++) {
		datapakker[resendPackage[i]].setResendBit();		// S�tter RPB'er (Resend Package Bit) for hver pakke der bliver gensendt. 
		abc = adddatapakke(resendPackage[i], abc);
	}
	/////	Send end	///////
	elementerIArray = abc * samples;
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
	//std::cout << "nakString: "<< nakString << std::endl;
	abc = makeSyncSequence();
	j = abc * samples;													// j er elementer i raw1
		////	Play string	///////
	for (int i = 0; i < nakString.length() / bitPrTone; i++) {
		acc = nakString.substr(i*bitPrTone, bitPrTone);					// Tager en del af stringen og laver den til en tone. 
		DTMFToner nakToner(acc);
		nakToner.setToneNumber();
		dtmfToner.push_back(nakToner);
	}
	std::cout << "dtmfToner size: " << dtmfToner.size() << std::endl;
	std::cout << "abc:  " << abc << std::endl;
	for (int i = abc; i < dtmfToner.size(); i++) {
		//std::cout << "dtmf-Tone nr: " << dtmfToner[i].getToneNumber() << std::endl;
		tone = dtmfToner[i].createTone(samples, sampleFreq);			// Tager et toneNr og konvertere det til en vector med amplituder
		for (int k = 0; k < samples; j++, k++) {
			raw1[j] = tone[k];
		}
	}
	elementerIArray = dtmfToner.size()*samples;
	return raw1;
}

sf::Int16* Afspilning::playSequence(int start, int antal)
{
	clearRaw1DTMF();
	int j = 0;
	int abc = 0;
	std::vector<float> tone;

    forrigeSendteDatapakker.clear();
    for (int i = 0; i < antal; i++) {
        forrigeSendteDatapakker.push_back(start + i);
    }
    

	abc = makeSyncSequence();
	j = abc * samples;

	/////	Send begin	///////
	sekvens.toDTMF(datapakker, dtmfToner, start, antal);								// Tager datapakker fra sekvensen startende fra start og giver det et toneNr. Dette g�r den 'antal' gange
	std::vector<float> toner;
	for (int dtmftoneNR = abc; dtmftoneNR < dtmfToner.size(); dtmftoneNR++) {
	toner = dtmfToner[dtmftoneNR].createTone(samples, sampleFreq);	// Tager et toneNr og konvertere det til en vector med amplituder
		for (int t = 0; t < samples; t++, j++) {
			raw1[j] = toner[t];															// Tilf�jer amplitudev�rdierne i afspilningsarrayet.
		}
	}
	/////	Send end	///////
	// F�r en pinter til en vector med protokol elementer (datapakker). Her skal funktionen afspille alle pakkerne mellem start og slut, inkl. gr�nserne.
	//Skal p� sigt implementere set LastPackageBit.
	elementerIArray = dtmfToner.size()*samples;
	return raw1;
}

sf::Int16 * Afspilning::playForrigePakker()
{
    clearRaw1DTMF();
    int abc = 0;
    abc = makeSyncSequence();
    /////	Send begin	///////
    for (int i = 0; i < forrigeSendteDatapakker.size(); i++) {
        datapakker[forrigeSendteDatapakker[i]].setResendBit();		// S�tter RPB'er (Resend Package Bit) for hver pakke der bliver gensendt. 
        abc = adddatapakke(forrigeSendteDatapakker[i], abc);
    }
    /////	Send end	///////
	elementerIArray = abc * samples;
    return raw1;
}

int Afspilning::adddatapakke(int pakke,int abc)
{
	/////	Send begin	///////
	sekvens.toDTMF(datapakker, dtmfToner, pakke, 1);									// Tager datapakker fra sekvensen startende fra start og giver det et toneNr. Dette g�r den 1 gang
	std::vector<float> toner;
	int j = abc * samples;


	for (int dtmftoneNR = abc; dtmftoneNR < dtmfToner.size(); dtmftoneNR++) {
		toner = dtmfToner[dtmftoneNR].createTone(samples, sampleFreq);					// Tager et toneNr og konvertere det til en vector med amplituder
		for (int t = 0; t < samples; t++, j++) {
			raw1[j] = toner[t];															// Tilf�jer amplitudev�rdierne i afspilningsarrayet.
		}
		abc++;
	}
	return abc; 
}

int Afspilning::makeSyncSequence()
{
	int abc = 0, j = 0;
	std::vector<float> tone;
	int perioder = 12;
	std::string sync;

	/////	sync begin	///////

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
		tone = dtmfToner[i].createTone(samples, sampleFreq);				// Vector af floats der forl�bende beskriver DTMFtonens amplitude
		for (int k = 0; k < samples; j++, k++) {
			raw1[j] = tone[k];
		}
		//abc++;
	}
	/////	sync end	///////
	abc++;
	return abc;									// retunere antallet af toner i afspilningsarrayet (raw�)
}


void Afspilning::clearRaw1DTMF()
{
	std::fill_n(raw1, arraySize, '0');			// t�mmer og nulstiller afspillingsarrayet (raw1)
	dtmfToner.clear();							// clearer vektoren der indeholder DTMF-tonerne
}

void Afspilning::setLastBit()
{
	datapakker[datapakker.size() - 1].setLastBit();
}

unsigned int Afspilning::getarraySize()
{
	return arraySize;
}

int Afspilning::getAntalDataPakker()
{
	return datapakker.size();
}

int Afspilning::getAntalDTMFToner()
{
	return dtmfToner.size();
}

int Afspilning::getAntalElementerIArray()
{
	return elementerIArray;
}


Afspilning::~Afspilning()
{
}
