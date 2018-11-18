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

void Synkronisering::startSync()
{
    bool keepSyncing = 1; // skal kunne sættes til 0 et sted for at stoppe tråden!
    int windowSz = (8000 * 20) / 1000; //I antal samples
    syncPtr = 0;
    int count = 0;
    while (keepSyncing == 1)

    {
        //Det tjekkes om mainBufferen har nok elementer til at der kan tages et vindue
        if (syncPtr + windowSz < mainPtr)
        {
            //Syncing



            //Outputting to string
        }



        std::cout << mainPtr << std::endl;
        std::mutex mu;
        if (mainPtr > 0)
        {
            mu.lock();
            std::cout << mainBuffer[1] << std::endl;
            mu.unlock();
        }
    }
}


Synkronisering::~Synkronisering()
{
}
