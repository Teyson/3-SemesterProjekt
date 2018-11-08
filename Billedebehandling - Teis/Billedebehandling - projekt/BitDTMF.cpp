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
	if (divisionsRest > 0)
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

void BitDTMF::toDTMF(std::vector<Protokol>& prot, std::vector<DTMFToner>& dtmfVec)
{
	int bitPrTone = 4;
	int antalOpdelinger;
	int divisionsrest;
	toneNr = 0;
	std::string bitHold;
	

	/*Opdeler hver protokol-objekt-streng til valgt tonest�rrelse. */
	const int size = prot.size();
	for (int i = 0; i < size; i++)
	{
		bitHold = prot[i].getString(); //Tager den protokoltilf�jede streng.

		//Opdeler strengen i bidder af bitPrTone
		stringLength = bitHold.length();			//L�ngden af den protkolstrengen
		antalOpdelinger = stringLength / bitPrTone; //Antal muligeopdelinger uden padding
		divisionsrest = stringLength % bitPrTone;	//Mulige opdelinger med padding
		if (divisionsrest > 0)
		{
			antalOpdelinger++;
			divisionsrest = bitPrTone - divisionsrest;
			bitHold = bitHold.append(divisionsrest, '0');//Padding tilf�jes s� der strengen kan deles en helt antal gange
		}



		/*Pusher hver enkel tone som objekt og s�tter tonestart og -slut*/
		std::string acc;
		prot[i].setToneStart(toneNr); //F�rste tonenr gemmes for protokollen
		for (int i = 1; i < antalOpdelinger + 1; i++)
		{
			acc = startString.substr((i * bitPrTone) - bitPrTone, bitPrTone);

			//Strengen pushes som objekt til klassen protokol 
			DTMFToner nyTone(acc);
			dtmfVec.push_back(nyTone);
			toneNr++;
		}
		prot[i].setToneSlut(toneNr); //Sidste tonenr gemmes for protokollen
	}
		//Dette loop skal opdele strengen af bits i dele af 4 eller 5
		//Pushe hver opdeling som et objekt, s�dan at hvert objekt er �n tone
		//Samtidig vil start- og sluttonenummeret gemmes i hvert objekt af protokollen
		//s�dan at tonerne nemt kan findes ved sendefejl.
		//DTMFToner klassen indeholder alts� alle toner

		//N�r strengen af bits er sendt, vil DTMFToner klassen lave en tone.


	

	bitHold = prot[0].getString();
	std::cout << bitHold << std::endl;

}


BitDTMF::~BitDTMF()
{
}
