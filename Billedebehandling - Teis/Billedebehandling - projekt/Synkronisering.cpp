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
    //std::mutex mu;

    for (int i = startPtr; i < vectorSize; i++)
    {
        mainBuffer.push_back(samples[i]);
    }
    mainPtr = mainBuffer.size();
    //std::cout << mainPtr << std::endl;



    //std::cout << mainBuffer.size() << std::endl;

}

void Synkronisering::sync()
{

    bool keepSyncing = 1; // skal kunne s�ttes til 0 et sted for at stoppe tr�den!
    bool startOutputting = 0; //N�r den er f�rdig med selve synkroniseringen, kan tonerne blive dekodet til bit

    int ms = 20;    //Vinduesst�rrelse i ms
    int fs = 8000;
    int windowSz = (fs * ms) / 1000; //vinduesst�rrelse i antal samples
    int forskydning = windowSz / (ms * 4);
    int low1;
    int high1;
    int low2;
    int high2;
    float fejlCount = 0;

    std::string sammenligning = "001101111111001010011000000111101011001010100101110101101100000000110111111100101001100000011110101100101010010111010110110000000011011111110010100110000001111010110010";

    int low1Amp = 20000;
    int high1Amp = 20000;
    int low2Amp = 20000;
    int high2Amp = 20000;
    int counter = 0;

    syncPtr = 0; //til at holde styr p�, hvad der er syncet i mainBuf
    int elementNr = 0; //Til at holde styr p� nr element der tages fra mainBuf

    Behandling behandling;
    DTMF2Bit d;

    float forhold1;
    float forhold2;
    float forhold3;
    bool doneSync = 0;
    int revertCounter = 0;
    int acc = 0;
    int previousAcc = 100;


    while (keepSyncing == 1)
    {
        //std::cout << mainPtr << std::endl;
        //std::cout << mainPtr << std::endl;
        //Det tjekkes om mainBufferen har nok elementer til at der kan tages et vindue

        if (syncPtr + windowSz < mainPtr)
        {
            //std::cout << syncPtr + windowSz << std::endl;
            //std::cout << mainPtr << std::endl;

            float gns1;
            float gns2;
            float forhold;
            //Syncing
            if (startOutputting == 0)
            {

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


                if (elementNr == 0) //F�rste gang, er vi interesseret i at f�rste tone er f�rdig inden tonevinduet slutter
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
            //Outputting to string
            else
            {
                for (size_t i = syncPtr; i < syncPtr + windowSz; i++)
                {
                    if (mainBuffer[i] < 0)
                        acc += mainBuffer[i] * (-1); //Ved negative værdier
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
                {
                    std::cout << "Modtagerside: Test af playString()  " << std::endl;
                    std::cout << "Den sendte bitstreng er:    001101111111001010011000000111101011001010100101110101101100000000110111111100101001100000011110101100101010010111010110110000000011011111110010100110000001111010110010" << std::endl;
                    bitstring.erase(bitstring.end() - 8, bitstring.end());

                    std::cout << "Den modtagede bitstreng er: " << bitstring << std::endl;

                    if (bitstring.size() > 166)
                    {
                        for (size_t i = 0; i < 168; i++)
                        {
                            if (bitstring[i] != sammenligning[i])
                                fejlCount++;
                        }
                        int procentFejl = fejlCount / (float)168 * 100;
                        std::cout << "Fejlprocenten er: " << procentFejl << std::endl;
                    }
                    else
                        std::cout << "Der var fejl i afsendingen" << std::endl;
                    
                    sf::sleep(sf::milliseconds(2000));
                }
                syncPtr += windowSz;
            }
        }

        sf::sleep(sf::milliseconds(5));
    }




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
