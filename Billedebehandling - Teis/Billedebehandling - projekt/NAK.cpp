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


std::string NAK::createNonEmptyNAK()
{
	std::string toSend;

	for (int i = pointerNotRecieved; i < pointerExpected; i++)
	{
		if (recieveArray[i] == "0")
		{
			std::bitset<4> bitset(i);
			toSend.append(bitset.to_string());
		}
	}
	return toSend;
}

std::string NAK::createEmptyNAK()
{
	return NAKTone;
}

std::string NAK::createNAK()
{
	std::string returnString;

	updatePointerNotRecieved();
	updatePointerMax();

	if (pointerExpected == pointerNotRecieved)
	{
		returnString = createEmptyNAK();
	}
	else
	{
		returnString = createNonEmptyNAK();
	}
	
	updatePointerExpected();

	return trailer(returnString);
}

std::string NAK::trailer(std::string s)
{
	CRC crc(s);

	std::string returnString = s + crc.crcCheck();

	return returnString;
}

void NAK::insertIntoArray(std::string s)
{
	int index = std::stoi(s, nullptr, 2);

	recieveArray[index] = s;
}

void NAK::initRecieveArray()
{
	//"0" forstås som null

	//Init recieveArray
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
		for (int i = pointerMax; i < recieveArray->size(); i++)
		{
			recieveArray[i] = "0";
		}
	}
}

void NAK::updatePointerNotRecieved()
{
	initRecieveArray();

	for (int i = pointerNotRecieved; i < pointerExpected; i++)
	{
		if (recieveArray[i] == "0")
		{
			pointerNotRecieved = i;
		}
	}

	if (recieveArray[pointerNotRecieved] != "0")
	{
		pointerNotRecieved = pointerExpected;
	}

	if (pointerNotRecieved == pointerExpected)
	{
		NAKBoolean = false;
	}
}

void NAK::updatePointerMax()
{
	//Init pointerMax
	pointerMax = (pointerNotRecieved + windowSize) % arraySize;
}

void NAK::updatePointerExpected()
{
	if (!NAKBoolean)
	{
		if (pointerExpected <= (pointerMax - packetsSend))
			/*Dette tjek er som det er fordi: Hvis pointerExpected er lig pointerMax må vi være ude i anden udløb af timeren
			på sender siden. Hvis pointerNotRecieved står på en plads der ikke er delelig med halvdelen af vinduet,
			vil vi være i en situation hvor der mangler nogle pakker. Derfor skal der ventes på at alle pakker er fremme
			inden vi går videre.*/
		{
			pointerExpected += packetsSend;
		}
		else
		{
			NAKBoolean = true;
		}
	}
}

void NAK::updateAllPointers()
{
	updatePointerNotRecieved();
	updatePointerMax();
	updatePointerExpected();
}

void NAK::reset()
{
	std::fill_n(recieveArray, arraySize, "0");

	pointerExpected = packetsSend;
	pointerMax = pointerNotRecieved + windowSize;
	pointerNotRecieved = 0;
	NAKBoolean = false;
}

int NAK::getPointerNotRecieved()
{
	return pointerNotRecieved;
}

int NAK::getPointerExpected()
{
	return pointerExpected;
}

int NAK::getPointerMax()
{
	return pointerMax;
}

bool NAK::getNAKBoolean()
{
	return NAKBoolean;
}

NAK::~NAK()
{
}
