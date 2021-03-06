#pragma once
#include <vector>
#include <string>
#include <mutex>
#include <iostream>
#include <SFML/Audio.hpp>
#include <conio.h>  //kbhit
#include <fstream>

#include "Behandling.h"
#include "DTMF2Bit.h"

class Synkronisering 
{
public:
	Synkronisering();
    void addToMainBuffer(const sf::Int16 *samples, int startPtr, int vectorSize);
    void clearMainBuffer(bool clear);
    void sync();
	std::string startThread();
	std::string getString();

	~Synkronisering();

protected:
    std::vector<int> syncBuffer;
    std::vector<int> mainBuffer;
    std::string bitstring;
  
    int a;
    int mainPtr;
    int syncPtr;

};