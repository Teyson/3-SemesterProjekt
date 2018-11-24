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

    int ms = 40;    //Vinduesst�rrelse i ms
    int fs = 8000;  
    int windowSz = (fs * ms) / 1000; //vinduesst�rrelse i antal samples
    int forskydning = windowSz / 10;
    int low1;
    int high1;
    int low2;
    int high2;

    int low1Amp = 20000;
    int high1Amp = 20000;
    int low2Amp = 20000;
    int high2Amp = 20000;
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
			
            float gns1;
            float gns2;
            float forhold;
            //Syncing
            if (startOutputting == 0)
            {

				if (elementNr == 19)
				{
					startOutputting = 1;
				}
                high1 = behandling.goertzler(fs, 1209, &mainBuffer, syncPtr, windowSz);
                low1 = behandling.goertzler(fs, 697, &mainBuffer, syncPtr, windowSz);
                high2 = behandling.goertzler(fs, 1633, &mainBuffer, syncPtr, windowSz);
                low2 = behandling.goertzler(fs, 941, &mainBuffer, syncPtr, windowSz);
                gns1 = ((float)high1 + (float)low1) / 2;
                gns2 = ((float)high2 + (float)low2) / 2;
                forhold = gns1 / gns2;

                //std::cout << forhold << std::endl;
                //std::cout << elementNr << std::endl;

                
                if (elementNr == 0) //F�rste gang, er vi interesseret i at f�rste tone er f�rdig inden tonevinduet slutter
                {
                    if (forhold < 9)
                    {
                        syncPtr += forskydning - (forskydning / 4) ;
                        //std::cout << forhold << std::endl;
                        //std::cout << "forskydning" << std::endl;
                    }
                }
                else if (elementNr % 2 == 0 && elementNr != 0)
                {
                    if (forhold < 9)
                    {
                        syncPtr += forskydning;
                     //   std::cout << forhold << std::endl;
                       // std::cout << "forskydning 2" << std::endl;
                    }
                }
                else
                {
                    if (forhold > 0.6)
                    {
                        syncPtr += forskydning;
                        //std::cout << forhold << std::endl;
                       // std::cout << "forskydning 3" << std::endl;
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

                float mistake = 0;
                std::string check = "0011011111110010100110000001111010110010101001011101011011000000";
                if (counter == 63)
                {
                    for (size_t i = 0; i < 64; i++)
                    {
                        if (bitstring[i] != check[i])
                           mistake ++;
                    }
                    std::cout << mistake / ((float)64) * 100 << std::endl;
                }
                counter++;
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
