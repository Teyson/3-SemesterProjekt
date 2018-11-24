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
    int forskydning = windowSz / 10;
    int low1;
    int high1;
    int low2;
    int high2;

    int low1Amp = 6000;
    int high1Amp = 6000;
    int low2Amp = 6000;
    int high2Amp = 6000;
    int counter = 0;
        
    syncPtr = 0; //til at holde styr p�, hvad der er syncet i mainBuf
    int elementNr = 0; //Til at holde styr p� nr element der tages fra mainBuf

    Behandling behandling;
    DTMF2Bit d;

    while (keepSyncing == 1)
    {
		//std::cout << mainPtr << std::endl;
        //std::cout << mainPtr << std::endl;
        //Det tjekkes om mainBufferen har nok elementer til at der kan tages et vindue
      
        if (syncPtr + windowSz < mainPtr)
        {
            //std::cout << syncPtr + windowSz << std::endl;
            //std::cout << mainPtr << std::endl;
			
            //Syncing
            if (startOutputting == 0)
            {

				if (elementNr == 9)
				{
					startOutputting = 1;
				}
                if (elementNr % 2 == 0)
                {
                    high1 = behandling.goertzler(fs, 1209, &mainBuffer, syncPtr, windowSz);
                    low1 = behandling.goertzler(fs, 697, &mainBuffer, syncPtr, windowSz);
                  //  std::cout << high1 << std::endl;
                 //   std::cout << low1 << std::endl;
                }
                else
                {
                    high2 = behandling.goertzler(fs, 1633, &mainBuffer, syncPtr, windowSz);
                    low2 = behandling.goertzler(fs, 941, &mainBuffer, syncPtr, windowSz);
                }

                
                if (elementNr == 0) //F�rste gang, er vi interesseret i at f�rste tone er f�rdig inden tonevinduet slutter
                {
                    if (high1 < high1Amp && low1 < low1Amp)
                    {
                        syncPtr += forskydning - (forskydning / 4) ;
                    }
                }
                else if (elementNr % 2 == 0 && elementNr != 0)
                {
                    if (high1 < high1Amp && low1 < low1Amp)
                    {
                        syncPtr += forskydning;
                    }
                }
                else
                {
                    if (high2 < high2Amp && low2 < low2Amp)
                    {
                        syncPtr += forskydning;
                    }
                }
                elementNr++;
                syncPtr += windowSz;
            }
            //Outputting to string
            else
            {
                bitstring.append(d.convertDTMF2Nibble(8000, &mainBuffer, syncPtr, windowSz));
                syncPtr += windowSz;
                counter++;
                if (counter == 5)
                {
                    std::cout << bitstring << std::endl;
                }
            }
        }
        
		sf::sleep(sf::milliseconds(10));
     }



        
}

void Synkronisering::startThread()
{
	std::thread syncLoader(&Synkronisering::sync, this);
	syncLoader.join();
}

Synkronisering::~Synkronisering()
{
}
