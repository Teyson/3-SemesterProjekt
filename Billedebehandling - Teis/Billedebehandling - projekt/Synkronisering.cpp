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
    bool keepSyncing = 1; // S�ttes til 0, n�r tr�den skal stoppes.
    bool startOutputting = 0; //N�r den er f�rdig med selve synkroniseringen, kan tonerne blive dekodet til bit.

    int ms = 20;    //Vinduesst�rrelse i ms
    int fs = 8000;	//Samplefrekvens
    int windowSz = (fs * ms) / 1000; //Vinduesst�rrelse i antal samples.
    int forskydning = windowSz / (ms * 4);	//Minimum forskydning.

    int high1;	//Gemmer Goertzler-v�rdien for 1209 Hz.
    int low1;	//Gemmer Goertzler-v�rdien for 697 Hz.
    int low2;	//Gemmer Goertzler-v�rdien for 1633 Hz.
    int high2;	//Gemmer Goertzler-v�rdien for 941 Hz.

    int counter = 0;

    syncPtr = 0; //Til at holde styr p�, hvad der er loadet ind fra mainbufferen.
    int elementNr = 0; //Til at holde styr p� antallet af synkroniseringstoner.

    Behandling behandling;
    DTMF2Bit d;

    float gns1;		// Gemmer gennemsnittet af Goertzel-v�rdier for 1209 Hz og 697 Hz.
    float gns2;		// Gemmer gennemsnittet af Goertzel-v�rdier for 1633 Hz og 941 Hz.
    float forhold;	// Gemmer det nuv�rende forhold mellem gns1 og gns2.

    float forhold1;		// Gemmer v�rdi for forrrige forhold.
    float forhold2;
    float forhold3;
    bool doneSync = 0;		// S�ttes hvis vi ikke l�ngere �nsker at forskyde vinduet.
    int revertCounter = 0;	 // Hvor mange gange vi har "revertet" en forskydning i tr�k.

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
                gns1 = ((float)high1 + (float)low1) / 2;		// Gennemsnit af Goertzlel-v�rdierne for tone 1.
                gns2 = ((float)high2 + (float)low2) / 2;		// Gennemsnit af Goertzel-v�rdierne for tone 2.
                forhold = gns1 / gns2;		// Nuv�rende forhold mellem tone 1 og tone 2.

                                            /*if (elementNr % 2 == 0) {
                                            std::cout << "1   " << forhold << std::endl;
                                            }
                                            else
                                            std::cout << forhold << std::endl;*/

                if (elementNr == 0) //G�r ind i dette loop, n�r den f�rste synkroniseringstone modtages.
                {
                    if (forhold < 0.5)
                        syncPtr += 20 * forskydning;
                    syncPtr += forskydning - (forskydning / 4);		//Vi vil gerne at den forskyder lidt mindre f�rste gang.
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
                    if (forhold < forhold3 && revertCounter < 2)		// Hvis det nuv�rende forhold er lavere end det forrige forhold, vil vi gerne "reverte" forskydningen.
                    {													// revertCounteren skal s�rge for at vi ikke kan reverte mere end 2 gange i tr�k.
                        syncPtr -= forskydning;
                        revertCounter++;
                        if (forhold > 10)
                            doneSync = true;		// Hvis forholdet er st�rre end 10, �nsker vi ikke at forskyde vinduet mere.
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

				if (elementNr == 22)
				{
					startOutputting = 1;
				}
				high1 = behandling.goertzler(fs, 1209, &mainBuffer, syncPtr, windowSz);
				low1 = behandling.goertzler(fs, 697, &mainBuffer, syncPtr, windowSz);
				high2 = behandling.goertzler(fs, 1633, &mainBuffer, syncPtr, windowSz);
				low2 = behandling.goertzler(fs, 941, &mainBuffer, syncPtr, windowSz);
				gns1 = ((float)high1 + (float)low1) / 2;
				gns2 = ((float)high2 + (float)low2) / 2;
				/*std::cout << "Gns tone 0:   " << gns1 << std::endl;
				std::cout << "Gns tone 15:   " << gns2 << std::endl;*/

				if (elementNr % 2 == 0) {
					forhold = gns1 / gns2;
				}
				else
					forhold = gns2 / gns1;
				//std::cout << forhold << std::endl;
				//std::cout << elementNr << std::endl;


				if (elementNr == 0) //F?rste gang, er vi interesseret i at f?rste tone er f?rdig inden tonevinduet slutter
				{
					if (forhold < 0.5)
						syncPtr += 20 * forskydning;
					syncPtr += forskydning - (forskydning / 4);
					forhold1 = forhold;

				}
				else if (elementNr == 1)
				{
					// std::cout << "elementnr2" << std::endl;
					forhold3 = forhold;
					forhold2 = forhold - forhold1;
					if (forhold2 > 0)
					{
						//    std::cout << "opadgaaende" << std::endl;
						if (forhold < 1) {
							syncPtr += 18 * forskydning;
							//  std::cout << "forskydning 18" << std::endl;
						}
						else if (forhold < 2) {
							syncPtr += 8 * forskydning;
							//std::cout << "forskydning 8" << std::endl;
						}
						else if (forhold < 4) {
							syncPtr += 5 * forskydning;
							// std::cout << "forskydning 5" << std::endl;
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

					else
					{
						//std::cout << "nedadgaaende" << std::endl;
						if (forhold > 4) {
							syncPtr += 65 * forskydning;
							//  std::cout << "forskydning 65 ned" << std::endl;
						}
						else if (forhold > 2) {
							syncPtr += 60 * forskydning;
							// std::cout << "forskydning 60 ned" << std::endl;
						}
						else if (forhold > 1) {
							syncPtr += 45 * forskydning;
							// std::cout << "forskydning 45 ned" << std::endl;
						}
						else {
							syncPtr += 40 * forskydning;
							// std::cout << "forskydning 40 ned" << std::endl;
						}
					}

				}
				else if (elementNr != 1 && elementNr != 0 && doneSync == false)
				{
					if (forhold < forhold3 && revertCounter < 2)
					{
						syncPtr -= forskydning;
						revertCounter++;
						if (forhold > 10)
							doneSync = true;
						// std::cout << "revert" << std::endl;
					}
					else if (forhold < 1) {
						syncPtr += 18 * forskydning;
						revertCounter = 0;
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
							// std::cout << "forskydning" << std::endl;
						}
						revertCounter = 0;
					}

					forhold3 = forhold;
				}
				elementNr++;
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
