#include "NAK.h"

NAK::NAK()
{
	recieveArray = new std::string[arraySize];
	recieveDataArray = new std::string[arraySize];


	for (int i = 0; i < arraySize; i++)
	{
		recieveArray[i] = "0";
	}

	//laver array der er datast�rrelsen * bufferst�rrelse skal f�rst initialiseres n�r der bliver modtaget en pakke der er har data.
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


	//laver array der er datast�rrelsen * bufferst�rrelse skal f�rst initialiseres n�r der bliver modtaget en pakke der er har data.
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
		//std::cout << "Created Empty NAK" << std::endl;
	}
	else
	{
		returnString = createNonEmptyNAK();
		//std::cout << "Created non-empty NAK" << std::endl;
	}
	
	

	return trailer(returnString);
}

std::string NAK::trailer(std::string s)
{
	CRC crc(s);

	std::string returnString = s + crc.crcCheck();

	return returnString;
}

void NAK::insertIntoArray(std::string s,std::string d) //s -> sekvensnr    d -> data
{
	//seq.nr. er en indeksering for vores string-array
	int index = std::stoi(s, nullptr, 2);
	

	recieveDataArray[index] = d;
	recieveArray[index] = s;
}

void NAK::initRecieveArray()
{
	//"0" forst�s som null

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
		for (int i = pointerMax; i < arraySize; i++)
		{
			recieveArray[i] = "0";
			recieveDataArray[i] = "";
		}
	}
}

void NAK::updatePointerNotRecieved()
{
    bool loopOver = false;
	int pointerStartValue = pointerNotRecieved;

    if (pointerNotRecieved > pointerExpected)
    {
        for (size_t i = pointerNotRecieved; i < arraySize; i++)
        {
            if (recieveArray[i] == "0")
            {
                pointerNotRecieved = i;
                loopOver = true;
                break;
            }
        }

        for (size_t i = 0; i < pointerExpected && !loopOver; i++)
        {
            if (recieveArray[i] == "0")
            {
                pointerNotRecieved = i;
                break;
            }
        }
    }
    else
    {
        for (size_t i = pointerNotRecieved; i < arraySize; i++)
        {
            if (recieveArray[i] == "0")
            {
                pointerNotRecieved = i;
                break;
            }
        }
    }

	if (recieveArray[pointerNotRecieved] != "0")
	{
		pointerNotRecieved = pointerExpected;
	}

	if (pointerNotRecieved == pointerExpected)  //NAKBoolean er true hvis der er negtative checksumme og false hvis alle checksumme er korrekte
	{
		NAKBoolean = false;    
	}

	int pointerNewValue = pointerNotRecieved;
	
	//her inds�ttes data i klassens variabel dataModtaget, hvis pointerNotRecived har rykket sig(der er blevet modtaget pakker).
	if (pointerStartValue != pointerNewValue)
	{
		for (int i = pointerStartValue; i < pointerNewValue; i++) 
		{
			std::string data = recieveDataArray[i];
			dataModtaget.append(data);
		}
	}
	
    updatePointerMax();
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

	pointerExpected = (pointerExpected + packetsSend) % arraySize;
	//if (!NAKBoolean)
	//{
	//	if (pointerMax - packetsSend < 0)
	//	{
	//		if (pointerExpected <= (pointerMax + arraySize - packetsSend))
	//		{
	//			pointerExpected += packetsSend;
	//		}
	//		else // nakBoolean s�ttes hvis pointer notExpected ikke kan blive h�jere
	//		{
	//			NAKBoolean = true;
	//		}
	//	}
	//	else
	//	{
	//		if (pointerExpected <= (pointerMax - packetsSend))
	//			/*Dette tjek er som det er fordi: Hvis pointerExpected er lig pointerMax m� vi v�re ude i anden udl�b af timeren
	//			p� sender siden. Hvis pointerNotRecieved st�r p� en plads der ikke er delelig med halvdelen af vinduet,
	//			vil vi v�re i en situation hvor der mangler nogle pakker. Derfor skal der ventes p� at alle pakker er fremme
	//			inden vi g�r videre.*/
	//		{
	//			pointerExpected += packetsSend;
	//		}
	//		else
	//		{
	//			NAKBoolean = true;
	//		}
	//	}
	//}
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
