#include "NAK.h"



NAK::NAK()
{
	recieveArray = new std::string[arraySize];

	for (int i = 0; i < arraySize; i++)
	{
		recieveArray[i] = "0";
	}


	windowSize = packetsSend * 2 + 1;
}

NAK::NAK(int arrayS, int packets)
{
	arraySize = arrayS;
	packetsSend = packets;

	recieveArray = new std::string[arraySize];

	for (int i = 0; i < arraySize; i++)
	{
		recieveArray[i] = "0";
	}

	windowSize = packetsSend * 2 + 1;
}


std::string NAK::createNAK()
{
	return "";
}

void NAK::insertIntoArray(std::string s)
{
	int index = std::stoi(s, nullptr, 2);

	recieveArray[index] = s;
}

void NAK::initPointers()
{
	//"0" forstås som null

	if (pointerMax < pointerNotRecieved)
	{
		for (int i = pointerMax; i < pointerNotRecieved; i++)
		{
			recieveArray[i] = "0";
		}
	}
	else if (pointerMax > pointerNotRecieved)
	{
		for (int i = 0; i < pointerNotRecieved; i++)
		{
			recieveArray[i] = "0";
		}
	}

	pointerMax = (pointerNotRecieved + windowSize + 1) % arraySize;
}

NAK::~NAK()
{
}
