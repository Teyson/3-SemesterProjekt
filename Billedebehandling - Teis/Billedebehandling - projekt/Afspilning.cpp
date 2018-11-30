#include "Afspilning.h"



Afspilning::Afspilning()
{
}

Afspilning::Afspilning(std::string datainput, int sampleInput, int sampleRateInput)
{
	raw1 = new sf::Int16[arraySize+antalSyncvaerdier+100];// En tone + antal sync + buffer
	raw2 = new sf::Int16[2*arraySize + antalSyncvaerdier + 100];
	raw3 = new sf::Int16[3 * arraySize + antalSyncvaerdier + 100];
	raw4 = new sf::Int16[4 * arraySize + antalSyncvaerdier + 100];
	raw5 = new sf::Int16[5 * arraySize + antalSyncvaerdier + 100];
	raw6 = new sf::Int16[6 * arraySize + antalSyncvaerdier + 100];
	rawNak = new sf::Int16[1280 + antalSyncvaerdier + 100]; // 1280 er maksimal antal nak værdier
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
	switch (resendPackage.size())
	{
	case 1: {
		abc = makeSyncSequence(1);
		/////	Send begin	///////
		for (int i = 0; i < resendPackage.size(); i++) {
			datapakker[resendPackage[i]].setResendBit();		// Sætter RPB'er (Resend Package Bit) for hver pakke der bliver gensendt. 
			abc = adddatapakke(resendPackage[i], abc,1);
		}
		/////	Send end	///////
		rawNumber = 1;
		return raw1;
	}
	case 2: {
		abc = makeSyncSequence(2);
		/////	Send begin	///////
		for (int i = 0; i < resendPackage.size(); i++) {
			datapakker[resendPackage[i]].setResendBit();		// Sætter RPB'er (Resend Package Bit) for hver pakke der bliver gensendt. 
			abc = adddatapakke(resendPackage[i], abc,2);
		}
		/////	Send end	///////
		rawNumber = 2;
		return raw2;
	}
	case 3: {
		abc = makeSyncSequence(3);
		/////	Send begin	///////
		for (int i = 0; i < resendPackage.size(); i++) {
			datapakker[resendPackage[i]].setResendBit();		// Sætter RPB'er (Resend Package Bit) for hver pakke der bliver gensendt. 
			abc = adddatapakke(resendPackage[i], abc,3);
		}
		/////	Send end	///////
		rawNumber = 3;
		return raw3;
	}
	case 4: {
		abc = makeSyncSequence(4);
		/////	Send begin	///////
		for (int i = 0; i < resendPackage.size(); i++) {
			datapakker[resendPackage[i]].setResendBit();		// Sætter RPB'er (Resend Package Bit) for hver pakke der bliver gensendt. 
			abc = adddatapakke(resendPackage[i], abc,4);
		}
		/////	Send end	///////
		rawNumber = 4;
		return raw4;
	}
	case 5: {
		abc = makeSyncSequence(5);
		/////	Send begin	///////
		for (int i = 0; i < resendPackage.size(); i++) {
			datapakker[resendPackage[i]].setResendBit();		// Sætter RPB'er (Resend Package Bit) for hver pakke der bliver gensendt. 
			abc = adddatapakke(resendPackage[i], abc,5);
		}
		/////	Send end	///////
		rawNumber = 5;
		return raw5;
	}
	case 6: {
		abc = makeSyncSequence(6);
		/////	Send begin	///////
		for (int i = 0; i < resendPackage.size(); i++) {
			datapakker[resendPackage[i]].setResendBit();		// Sætter RPB'er (Resend Package Bit) for hver pakke der bliver gensendt. 
			abc = adddatapakke(resendPackage[i], abc,6);
		}
		/////	Send end	///////
		rawNumber = 6;
		return raw6;
	}
	default:
		std::cout << "Well you need some more cases ;) " << std::endl;
		break;
	}
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
			rawNak[j] = tone[k];
		}
	}
	rawNumber = 0;
	return rawNak;
}

sf::Int16* Afspilning::playSequence(int start, int antal)
{
	clearRaw1DTMF();
	int j = 0;
	int abc = 0;
	std::vector<float> tone;


	abc = makeSyncSequence(3);
	j = abc * samples;

	/////	Send begin	///////
	sekvens.toDTMF(datapakker, dtmfToner, start, antal);								// Tager datapakker fra sekvensen startende fra start og giver det et toneNr. Dette gør den 'antal' gange
	std::vector<float> toner;
	int z = 0;
	for (int dtmftoneNR = abc; dtmftoneNR < dtmfToner.size(); dtmftoneNR++) {
		toner = dtmfToner[dtmftoneNR].createTone(samples, sampleFreq);					// Tager et toneNr og konvertere det til en vector med amplituder
		for (int t = 0; t < samples; t++, j++) {
			raw3[j] = toner[t];															// Tilføjer amplitudeværdierne i afspilningsarrayet.
		}
		z = dtmftoneNR - abc;
	}
	/////	Send end	///////
	std::cout << "cout af z: " << z << std::endl;
	// Får en pinter til en vector med protokol elementer (datapakker). Her skal funktionen afspille alle pakkerne mellem start og slut, inkl. grænserne.
	//Skal på sigt implementere set LastPackageBit. 
	rawNumber = 3;
	return raw3;
}

int Afspilning::adddatapakke(int pakke,int abc, int rawµ)
{
	/////	Send begin	///////
	sekvens.toDTMF(datapakker, dtmfToner, pakke, 1);									// Tager datapakker fra sekvensen startende fra start og giver det et toneNr. Dette gør den 1 gang
	std::vector<float> toner;
	int j = abc * samples;

	switch (rawµ)
	{
	case 1: {
		for (int dtmftoneNR = abc; dtmftoneNR < dtmfToner.size(); dtmftoneNR++) {
			toner = dtmfToner[dtmftoneNR].createTone(samples, sampleFreq);					// Tager et toneNr og konvertere det til en vector med amplituder
			for (int t = 0; t < samples; t++, j++) {
				raw1[j] = toner[t];															// Tilføjer amplitudeværdierne i afspilningsarrayet.
			}
			abc++;
		}
		rawNumber = 1;
		return abc; 
	}
	case 2: {
		for (int dtmftoneNR = abc; dtmftoneNR < dtmfToner.size(); dtmftoneNR++) {
			toner = dtmfToner[dtmftoneNR].createTone(samples, sampleFreq);					// Tager et toneNr og konvertere det til en vector med amplituder
			for (int t = 0; t < samples; t++, j++) {
				raw2[j] = toner[t];															// Tilføjer amplitudeværdierne i afspilningsarrayet.
			}
			abc++;
		}
		rawNumber = 2;
		return abc;
	}
	case 3: {
		for (int dtmftoneNR = abc; dtmftoneNR < dtmfToner.size(); dtmftoneNR++) {
			toner = dtmfToner[dtmftoneNR].createTone(samples, sampleFreq);					// Tager et toneNr og konvertere det til en vector med amplituder
			for (int t = 0; t < samples; t++, j++) {
				raw3[j] = toner[t];															// Tilføjer amplitudeværdierne i afspilningsarrayet.
			}
			abc++;
		}
		rawNumber = 3;
		return abc;
	}
	case 4: {
		for (int dtmftoneNR = abc; dtmftoneNR < dtmfToner.size(); dtmftoneNR++) {
			toner = dtmfToner[dtmftoneNR].createTone(samples, sampleFreq);					// Tager et toneNr og konvertere det til en vector med amplituder
			for (int t = 0; t < samples; t++, j++) {
				raw4[j] = toner[t];															// Tilføjer amplitudeværdierne i afspilningsarrayet.
			}
			abc++;
		}
		rawNumber = 4;
		return abc;
	}
	case 5:
	{
		for (int dtmftoneNR = abc; dtmftoneNR < dtmfToner.size(); dtmftoneNR++) {
			toner = dtmfToner[dtmftoneNR].createTone(samples, sampleFreq);					// Tager et toneNr og konvertere det til en vector med amplituder
			for (int t = 0; t < samples; t++, j++) {
				raw5[j] = toner[t];															// Tilføjer amplitudeværdierne i afspilningsarrayet.
			}
			abc++;
		}
		rawNumber = 5;
		return abc;
	}
	case 6: {
		for (int dtmftoneNR = abc; dtmftoneNR < dtmfToner.size(); dtmftoneNR++) {
			toner = dtmfToner[dtmftoneNR].createTone(samples, sampleFreq);					// Tager et toneNr og konvertere det til en vector med amplituder
			for (int t = 0; t < samples; t++, j++) {
				raw6[j] = toner[t];															// Tilføjer amplitudeværdierne i afspilningsarrayet.
			}
			abc++;
		}
		rawNumber = 6;
		return abc;
	}
	default:
		std::cout << "Well you need more cases in adddatapakke ;)" << std::endl;
		break;
	}
	
	/////	Send end	///////

}

int Afspilning::makeSyncSequence(int rawµ)
{
	int abc = 0, j = 0;
	std::vector<float> tone;
	int perioder = 20;
	std::string sync;

	/////	sync begin	///////
	switch (rawµ)
	{
	case 0: {
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
				rawNak[j] = tone[k];
			}
			abc++;
		}
		/////	sync end	///////
		abc++;
		rawNumber = 0;
		return abc;									// retunere antallet af toner i afspilningsarrayet (rawµ)
	}
	case 1: {
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
			abc++;
		}
		/////	sync end	///////
		abc++;
		rawNumber = 1;
		return abc;									// retunere antallet af toner i afspilningsarrayet (rawµ)
	}
	case 2: {
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
				raw2[j] = tone[k];
			}
			abc++;
		}
		/////	sync end	///////
		abc++;
		rawNumber = 2;
		return abc;									// retunere antallet af toner i afspilningsarrayet (rawµ)
	}
	case 3: {
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
				raw3[j] = tone[k];
			}
			abc++;
		}
		/////	sync end	///////
		abc++;
		rawNumber = 3;
		return abc;									// retunere antallet af toner i afspilningsarrayet (rawµ)
	}
	case 4: {
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
				raw4[j] = tone[k];
			}
			abc++;
		}
		/////	sync end	///////
		abc++;
		rawNumber = 4;
		return abc;									// retunere antallet af toner i afspilningsarrayet (rawµ)
	}
	case 5: {
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
				raw5[j] = tone[k];
			}
			abc++;
		}
		/////	sync end	///////
		abc++;
		rawNumber = 5;
		return abc;									// retunere antallet af toner i afspilningsarrayet (rawµ)
	}
	case 6:
	{
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
				raw6[j] = tone[k];
			}
			abc++;
		}
		/////	sync end	///////
		abc++;
		rawNumber = 6;
		return abc;									// retunere antallet af toner i afspilningsarrayet (rawµ)
	}
	default:
		std::cout << "Well you need some more cases in makeSyncSeq ;)" << std::endl;
		break;
	}						// retunere antallet af toner i afspilningsarrayet (raw1)
}

void Afspilning::clearRaw1DTMF()
{
	std::fill_n(raw1, arraySize, 0);			// tømmer og nulstiller afspillingsarrayet (raw1)
	dtmfToner.clear();							// clearer vektoren der indeholder DTMF-tonerne
}

void Afspilning::setLastBit()
{
	datapakker[datapakker.size() - 1].setLastBit();
}

unsigned int Afspilning::getarraySize()
{
		switch (rawNumber)
	{
	case 0:
		return 1280 + antalSyncvaerdier + 100;
	case 1:
		return arraySize + antalSyncvaerdier + 100;
	case 2:
		return 2 * arraySize + antalSyncvaerdier + 100;
	case 3: 
		return 3 * arraySize + antalSyncvaerdier + 100;
	case 4: 
		return 4 * arraySize + antalSyncvaerdier + 100;
	case 5:
		return 5 * arraySize + antalSyncvaerdier + 100;
	case 6:
		return 6 * arraySize + antalSyncvaerdier + 100;
	default:
		std::cout << "well you fucked up" << std::endl;
		break;
	}
}

int Afspilning::getAntalDataPakker()
{
	return datapakker.size();
}

int Afspilning::getAntalDTMFToner()
{
	return dtmfToner.size();
}


Afspilning::~Afspilning()
{
}

