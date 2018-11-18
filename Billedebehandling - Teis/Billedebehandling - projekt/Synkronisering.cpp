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
    std::mutex mu;
    
    for (int i = startPtr; i < vectorSize; i++)
    {
        mainBuffer.push_back(samples[i]);
    }
    mainPtr = mainBuffer.size();
    
    
    
    //std::cout << mainBuffer.size() << std::endl;

}

void Synkronisering::sync()
{
    bool keepSyncing = 1; // skal kunne sættes til 0 et sted for at stoppe tråden!
    bool startOutputting = 0; //Når den er færdig med selve synkroniseringen, kan tonerne blive dekodet til bit

    int ms = 20;    //Vinduesstørrelse i ms
    int fs = 8000;  
    int windowSz = (fs * ms) / 1000; //vinduesstørrelse i antal samples
    int forskydning = windowSz / 10;
    int low1;
    int high1;
    int low2;
    int high2;

    int low1Amp = 5000;
    int high1Amp = 5000;
    int low2Amp = 5000;
    int high2Amp = 5000;
        
    syncPtr = 0; //til at holde styr på, hvad der er syncet i mainBuf
    int elementNr = 0; //Til at holde styr på nr element der tages fra mainBuf

    Behandling behandling;
    DTMF2Bit d;

    while (keepSyncing == 1)
    {
        //Det tjekkes om mainBufferen har nok elementer til at der kan tages et vindue
        if (syncPtr + windowSz < mainPtr)
        {
            if (elementNr == 9)
                startOutputting = 1;
            //Syncing
            if (startOutputting == 0)
            {
                if (elementNr % 2 == 0)
                {
                    high1 = behandling.goertzler(fs, 1209, &mainBuffer, syncPtr, syncPtr + windowSz);
                    low1 = behandling.goertzler(fs, 697, &mainBuffer, syncPtr, syncPtr + windowSz);
                }
                else
                {
                    high2 = behandling.goertzler(fs, 1633, &mainBuffer, syncPtr, syncPtr + windowSz);
                    low2 = behandling.goertzler(fs, 941, &mainBuffer, syncPtr, syncPtr + windowSz);
                }

                
                if (elementNr == 0) //Første gang, er vi interesseret i at første tone er færdig inden tonevinduet slutter
                {
                    if (high1 < high1Amp || low1 < low1Amp)
                    {
                        syncPtr += forskydning - (forskydning / 4) ;
                    }
                }
                else if (elementNr % 2 == 0 && elementNr != 0)
                {
                    if (high1 < high1Amp || low1 < low1Amp)
                    {
                        syncPtr += forskydning;
                    }
                }
                else
                {
                    if (high2 < high2Amp || low2 < low2Amp)
                    {
                        syncPtr += forskydning;
                    }
                }


                elementNr++;
            }
            //Outputting to string
            else
            {
                bitstring.append(d.convertDTMF2Nibble(8000, &mainBuffer, syncPtr, windowSz));
                syncPtr += windowSz;
            }
        }
        
       
     }



        
}


Synkronisering::~Synkronisering()
{
}
