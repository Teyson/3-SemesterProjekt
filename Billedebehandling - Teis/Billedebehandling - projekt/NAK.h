#pragma once
#include <vector>
#include <string>
#include <array>
#include "Protokol.h"

class NAK
{
public:
	NAK();
	NAK(int arrayS, int packets);
	
	std::string createNAK();
	void insertIntoArray(std::string s);
	void initPointers();

	~NAK();

protected:
	int pointerMax = 0;
	int pointerNotRecieved = 0;
	int pointerExpected = 0;
	int arraySize = 16;
	int windowSize;
	int packetsSend = 3;

	std::string* recieveArray;
};

