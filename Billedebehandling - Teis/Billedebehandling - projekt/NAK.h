#pragma once
#include <vector>
#include <string>
#include <array>
#include "Protokol.h"
#include <bitset>

class NAK
{
public:
	NAK();
	NAK(int arrayS, int packets);
	
	std::string createNonEmptyNAK();
	std::string createEmptyNAK();
	std::string createNAK();
	std::string trailer(std::string s);
	void insertIntoArray(std::string s);
	void insertIntoDataArray(std::string s, std::string d);
	void initRecieveArray();
	void updatePointerNotRecieved();
	void updatePointerMax();
	void updatePointerExpected();
	void updateAllPointers();
	void reset(); //Kun til test formål
	//Simple get-funktioner
	int getPointerNotRecieved();
	int getPointerExpected();
	int getPointerMax();
	std::string getDataFromArray(std::string s);
	bool getNAKBoolean();

	~NAK();

protected:
	int pointerMax = pointerNotRecieved + windowSize;
	int pointerNotRecieved = 0;
	int pointerExpected = packetsSend;
	int arraySize = 15;
	int windowSize;
	int packetsSend = 3; //Må maks være 7 med en arraystørrelse på 15. Udregning: (arraySize - 1) / 2 = maxPacketsSend!
	std::string NAKTone = "1111";
	bool NAKBoolean = false;

	std::string* recieveArray;
	std::string recieveDataAray;
};

