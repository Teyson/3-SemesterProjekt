#include "Synkronisering.h"



Synkronisering::Synkronisering()
{
}

void Synkronisering::clearMainBuffer(bool c)
{
	if (c == true)
		mainBuffer.clear();
}

void Synkronisering::addToMainBuffer(const sf::Int16 *samples, int startPtr, int vectorSize)
{

	for (int i = startPtr; i < vectorSize; i++)
	{
		mainBuffer.push_back(samples[i]);
	}
	mainPtr = mainBuffer.size();
}

void Synkronisering::sync()
{
	bool keepSyncing = 1; // Sættes til 0, når tråden skal stoppes.
	bool startOutputting = 0; //Når den er færdig med selve synkroniseringen, kan tonerne blive dekodet til bit.

	int ms = 20;    //Vinduesstørrelse i ms
	int fs = 8000;	//Samplefrekvens
	int windowSz = (fs * ms) / 1000; //Vinduesstørrelse i antal samples.
	int forskydning = windowSz / (ms * 4);	//Minimum forskydning.

	int high1;	//Gemmer Goertzler-værdien for 1209 Hz.
	int low1;	//Gemmer Goertzler-værdien for 697 Hz.
	int low2;	//Gemmer Goertzler-værdien for 1633 Hz.
	int high2;	//Gemmer Goertzler-værdien for 941 Hz.

	int counter = 0;

	syncPtr = 0; //Til at holde styr på, hvad der er loadet ind fra mainbufferen.
	int elementNr = 0; //Til at holde styr på antallet af synkroniseringstoner.

	Behandling behandling;
	DTMF2Bit d;

	float gns1;		// Gemmer gennemsnittet af Goertzel-værdier for 1209 Hz og 697 Hz.
	float gns2;		// Gemmer gennemsnittet af Goertzel-værdier for 1633 Hz og 941 Hz.
	float forhold;	// Gemmer det nuværende forhold mellem gns1 og gns2.

	float forhold1;		// Gemmer værdi for forrrige forhold.
	float forhold2;
	float forhold3;
	bool doneSync = 0;		// Sættes hvis vi ikke længere ønsker at forskyde vinduet.
	int revertCounter = 0;	 // Hvor mange gange vi har "revertet" en forskydning i træk.

	int acc = 100;
	int previousAcc = 0;

	while (keepSyncing == 1)
	{
		if (syncPtr + windowSz < mainPtr)	// Det tjekkes om mainBufferen har nok elementer til at der kan tages et vindue.
		{

			if (startOutputting == 0)	//Syncing.
			{
				high1 = behandling.goertzler(fs, 1209, &mainBuffer, syncPtr, windowSz);		//Goertzler 1209 Hz.
				low1 = behandling.goertzler(fs, 697, &mainBuffer, syncPtr, windowSz);		//Goertzler 697 Hz.
				high2 = behandling.goertzler(fs, 1633, &mainBuffer, syncPtr, windowSz);		//Goertzler 1633 Hz.
				low2 = behandling.goertzler(fs, 941, &mainBuffer, syncPtr, windowSz);		//Goertzler 941 Hz.
				gns1 = ((float)high1 + (float)low1) / 2;		// Gennemsnit af Goertzlel-værdierne for tone 1.
				gns2 = ((float)high2 + (float)low2) / 2;		// Gennemsnit af Goertzel-værdierne for tone 2.
				forhold = gns1 / gns2;		// Nuværende forhold mellem tone 1 og tone 2.

											/*if (elementNr % 2 == 0) {
											std::cout << "1   " << forhold << std::endl;
											}
											else
											std::cout << forhold << std::endl;*/

				if (elementNr == 0) //Går ind i dette loop, når den første synkroniseringstone modtages.
				{
					if (forhold < 0.5)
						syncPtr += 20 * forskydning;
					syncPtr += forskydning - (forskydning / 4);		//Vi vil gerne at den forskyder lidt mindre første gang.
					forhold1 = forhold;
				}
				else if (elementNr == 2)	//Vi vil gerne vide om forholdet er aftagende eller stigende for synkroniseringstone nr. 3
				{
					//std::cout << "elementnr2" << std::endl;
					forhold3 = forhold;
					forhold2 = forhold - forhold1;
					if (forhold2 > 0)		//Hvis forholdet er stigende.
					{
						//std::cout << "opadgaaende" << std::endl;
						if (forhold < 1) {
							syncPtr += 18 * forskydning;
							//std::cout << "forskydning 18" << std::endl;
						}
						else if (forhold < 2) {
							syncPtr += 8 * forskydning;
							//std::cout << "forskydning 8" << std::endl;
						}
						else if (forhold < 4) {
							syncPtr += 5 * forskydning;
							//std::cout << "forskydning 5" << std::endl;
						}
						else if (forhold < 6) {
							syncPtr += 3 * forskydning;
							//std::cout << "forskydning 3" << std::endl;
						}
						else {
							syncPtr += forskydning;
							//std::cout << "forskydning" << std::endl;
						}
					}

					else		// Hvis forholdet er aftagende, skal den forskydes mere.
					{
						std::cout << "nedadgaaende" << std::endl;
						if (forhold > 4) {
							syncPtr += 65 * forskydning;
							//std::cout << "forskydning 65 ned" << std::endl;
						}
						else if (forhold > 2) {
							syncPtr += 60 * forskydning;
							//std::cout << "forskydning 60 ned" << std::endl;
						}
						else if (forhold > 1) {
							syncPtr += 45 * forskydning;
							//std::cout << "forskydning 45 ned" << std::endl;
						}
						else {
							syncPtr += 40 * forskydning;
							//std::cout << "forskydning 40 ned" << std::endl;
						}
					}

				}
				else if (elementNr % 2 == 0 && elementNr != 2 && elementNr != 0 && doneSync == false)
				{
					if (forhold < forhold3 && revertCounter < 2)		// Hvis det nuværende forhold er lavere end det forrige forhold, vil vi gerne "reverte" forskydningen.
					{													// revertCounteren skal sørge for at vi ikke kan reverte mere end 2 gange i træk.
						syncPtr -= forskydning;
						revertCounter++;
						if (forhold > 10)
							doneSync = true;		// Hvis forholdet er større end 10, ønsker vi ikke at forskyde vinduet mere.
													//std::cout << "revert" << std::endl;
					}
					else if (forhold < 1) {
						syncPtr += 18 * forskydning;
						revertCounter = 0;			//Nulstiller revertCounter, da forholdet er voksende.
													//std::cout << "forskydning 18" << std::endl;
					}
					else if (forhold < 2) {
						syncPtr += 8 * forskydning;
						revertCounter = 0;
						//std::cout << "forskydning 8" << std::endl;
					}
					else if (forhold < 4) {
						syncPtr += 5 * forskydning;
						revertCounter = 0;
						//std::cout << "forskydning 5" << std::endl;
					}
					else if (forhold < 6) {
						syncPtr += 3 * forskydning;
						revertCounter = 0;
						//std::cout << "forskydning 3" << std::endl;
					}
					else {
						if (forhold > 10)
							doneSync = true;
						else {
							syncPtr += forskydning * 2;
							//std::cout << "forskydning" << std::endl;
						}
						revertCounter = 0;			//Nulstiller revertCounter, da forholdet er voksende.
					}
                   forhold3 = forhold;
				}

				if (elementNr == 37)		// Tjekker om vi har modtaget alle synkroniseringstonerne.
				{
					startOutputting = 1;
				}

				elementNr++;
				syncPtr += windowSz;		// Flytter syncPtr med vinduestørrelsen.
			}


			/////////Færdig med synkronisering//////////////
			else
			{
				for (size_t i = syncPtr; i < syncPtr + windowSz; i++)
				{
					if (mainBuffer[i] < 0)
						acc += mainBuffer[i] * (-1);
					else
						acc += mainBuffer[i];
				}
				acc = acc / windowSz; //Tager gns
				//std::cout << acc << std::endl;
			

				if (acc < previousAcc && acc < 45 && previousAcc < 45)
					keepSyncing = 0;

				previousAcc = acc;
				//std::cout << bitstring << std::endl;


				counter++;
				bitstring.append(d.convertDTMF2Nibble(8000, &mainBuffer, syncPtr, windowSz)); //Konverterer DTMF til bits og appender det til en string.
				if (keepSyncing == 0)
					bitstring.erase(bitstring.end() - 8, bitstring.end());
				syncPtr += windowSz;
			}
		}
		//sf::sleep(sf::milliseconds(5));
	}


    syncBuffer.clear();

}

std::string Synkronisering::startThread()
{
    std::thread syncLoader(&Synkronisering::sync, this);
	syncLoader.join();

	return bitstring;
}

std::string Synkronisering::getString()
{
	return bitstring;
}

Synkronisering::~Synkronisering()
{
}
