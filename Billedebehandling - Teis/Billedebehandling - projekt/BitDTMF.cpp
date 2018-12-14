#include "BitDTMF.h"

BitDTMF::BitDTMF()
{
}

BitDTMF::BitDTMF(std::string s,int samples, int samplingFrekvens, int opdelingsZ)
{
	opdelingsSize = opdelingsZ;
	startString = s;
	antalSamples = samples;
	fs = samplingFrekvens;
	stringLength = startString.size();
}


void BitDTMF::toProtokol(std::vector<Protokol>& p)
{
	int antalOpdelinger;
	int divisionsRest;

	using std::cout;
	using std::endl;

	//Unders�g hvor mange bidder stringen kan deles op i
	antalOpdelinger = stringLength / opdelingsSize;

	//Hver opdeling skal have opdelingsSize elementer, derfor tages der modulus til l�ngden af stringen
	divisionsRest = stringLength % opdelingsSize;
	if (divisionsRest != 0)
	{
		antalOpdelinger++;

		//Stringen skal gerne have en modulus v�rdi p� 0, derfor zero-paddes der p� de resterende elementer
		divisionsRest = opdelingsSize - divisionsRest;
		startString = startString.append(divisionsRest, '0');
	}

	//startString opdeles nu i opdelingsSize og gemmes i hvert sit objekt af typen Protokol i vektor protokoller
	std::string acc;
	for (int i = 1; i < antalOpdelinger + 1; i++)
	{
		acc = startString.substr((i * opdelingsSize) - opdelingsSize, opdelingsSize);
		
		//Strengen pushes som objekt til klassen protokol 
		Protokol nyProtokol(acc);
		p.push_back(nyProtokol);
	}
}

void BitDTMF::toDTMF(std::vector<Protokol>& prot, std::vector<DTMFToner>& dtmfVec,int begin, int antal)//start slut
{
	int bitPrTone = 4;
	int antalOpdelinger;
	int divisionsrest;
	toneNr = 0;
	std::string bitHold;
	
	
	/*Opdeler hver protokol-objekt-streng til valgt tonest�rrelse. */

	for (int i = begin; i < begin + antal; i++)
	{													//F�rst finder vi ud af hvor mange toner der er pr datapakke
		bitHold = prot[i].getString();			//Er der fejl her, s� skyldes det ofte at du �nsker et st�rre antal datapakker, end der kan dannes af r�dataen. 
		stringLength = bitHold.length();
		antalOpdelinger = stringLength / bitPrTone;
		divisionsrest = stringLength % bitPrTone;

		if (divisionsrest > 0) {						// i de tilf�lde hvor det ikke g�r lige op tilf�jes '0' indtil det passer
			antalOpdelinger++;
			divisionsrest = bitPrTone - divisionsrest;
			bitHold.append(divisionsrest, '0');
		}

		prot[i].setToneStart(toneNr);					// Angiver datapakkens f�rste tone nr. derved kan man nemmere finde hvilke toner der skal spilles, hvis denne pakke skal gensendes
		std::string acc;

		for (int t = 0; t < antalOpdelinger; t++)		// Giver hver pakke et tone nr. hvorfra man senere kan generere tonen ud fra tone nr.
		{
			acc = bitHold.substr(t*bitPrTone, bitPrTone);	// Finder en opdeling
			DTMFToner nyTone(acc);
			nyTone.setToneNumber();
			dtmfVec.push_back(nyTone);
			toneNr++;
		}

		prot[i].setToneSlut(toneNr - 1);				// angiver datapakkens sidste tone nr. 
	}

		/*Dette loop skal opdele strengen af bits i dele af 4 eller 5
		Pusher hver opdeling som et objekt, s�dan at hvert objekt er �n tone
		Samtidig vil start- og sluttonenummeret gemmes i hvert objekt af protokollen
		s�dan at tonerne nemt kan findes ved sendefejl.
		DTMFToner klassen indeholder alts� alle toner

		N�r strengen af bits er sendt, vil DTMFToner klassen lave en tone.*/
}

BitDTMF::~BitDTMF()
{
}
