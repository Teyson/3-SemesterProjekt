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
	void insertIntoArray(std::string s, std::string d);


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
	bool getNAKBoolean();
	std::string getDataFromArray(std::string s);
	std::string getDataModtaget();

	~NAK();

protected:
	int pointerNotRecieved = 0;
	int packetsSend = 3; //Må maks være 7 med en arraystørrelse på 15. Udregning: (arraySize - 1) / 2 = maxPacketsSend!
	int pointerExpected = packetsSend;
	int arraySize = 15;
	int windowSize = packetsSend * 2 + 1;
	int pointerMax = pointerNotRecieved + windowSize;
	std::string NAKTone = "1111";
	bool NAKBoolean = false;
	std::string dataModtaget;


	std::string* recieveArray;
	std::string* recieveDataArray;
};
