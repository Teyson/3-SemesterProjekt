#include "NAK.h"

NAK::NAK()
{
	recieveArray = new std::string[arraySize];
	recieveDataArray = new std::string[arraySize];


	for (int i = 0; i < arraySize; i++)
	{
		recieveArray[i] = "0";
	}

	//laver array der er datastørrelsen * bufferstørrelse skal først initialiseres når der bliver modtaget en pakke der er har data.
	for (int i = 0; i < arraySize; i++)
	{
		recieveDataArray[i] = "";
	}
}

NAK::NAK(int arrayS, int packets)
{
	arraySize = arrayS;
	packetsSend = packets;

	recieveArray = new std::string[arraySize];
	recieveDataArray = new std::string[arraySize];

	for (int i = 0; i < arraySize; i++)
	{
		recieveArray[i] = "0";
	}

	

	windowSize = packetsSend * 2 + 1;


	//laver array der er datastørrelsen * bufferstørrelse skal først initialiseres når der bliver modtaget en pakke der er har data.
	for (int i = 0; i < arraySize; i++)
	{
		recieveDataArray[i] = "";
	}
}



std::string NAK::getDataFromArray(std::string s)
{
	int index = std::stoi(s, nullptr, 2);

	std::string oensketStr = recieveDataArray[index];

	return oensketStr;
}

std::string NAK::getDataModtaget()
{
	return dataModtaget;
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
		std::cout << "Created Empty NAK" << std::endl;
	}
	else
	{
		returnString = createNonEmptyNAK();
		std::cout << "created nonempty NAK" << std::endl;
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

void NAK::insertIntoArray(std::string s,std::string d)
{
	//seq.nr. er en indeksering for vores string-array
	int index = std::stoi(s, nullptr, 2);
	

	recieveDataArray[index] = d;
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
			recieveDataArray[i] = "";
		}
	}
	else if (pointerMax > pointerNotRecieved)
	{
		for (int i = 0; i < pointerNotRecieved; i++)
		{
			recieveArray[i] = "0";
			recieveDataArray[i] = "";
		}
		for (int i = pointerMax; i < recieveArray->size(); i++)
		{
			recieveArray[i] = "0";
			recieveDataArray[i] = "";
		}
	}
}

void NAK::updatePointerNotRecieved()
{

	int pointerStartValue = pointerNotRecieved;

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

	int pointerNewValue = pointerNotRecieved;
	
	//her indsættes data i klassens variabel dataModtaget, hvis pointerNotRecived har rykket sig(der er blevet modtaget pakker).
	if (pointerStartValue != pointerNewValue)
	{
		for (int i = pointerStartValue; i < pointerNewValue; i++)
		{
			std::string data = recieveDataArray[i];
			dataModtaget.append(data);
		}
	}
	
	
	
	//her opdateres pointere
	initRecieveArray();
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
		if (pointerMax - packetsSend < 0)
		{
			if (pointerExpected <= (pointerMax + arraySize - packetsSend))
			{
				pointerExpected = (pointerExpected + packetsSend) % 15;
			}
			else // nakBoolean sættes hvis pointer notExpected ikke kan blive højere
			{
				NAKBoolean = true;
			}
		}
		else
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
