#include "Protokol.h"


Protokol::Protokol()
{
}

Protokol::Protokol(std::string s)
{
	startString = s;
	endString = startString;
	dataSize = 40;
}

std::string Protokol::getChecksum()
{
	return checksum;
}

std::string Protokol::getLastBit()
{
	return lastBit;
}

std::string Protokol::getSequenceNumber()
{
	return sequenceNumber;
}

std::string Protokol::getRecievedData()
{
	return recievedData;
}

void Protokol::setToneStart(int s)
{
	toneNrStart = s;
}

void Protokol::setToneSlut(int s)
{
	toneNrSlut = s;
}

std::string Protokol::getString()
{
	return endString;
}

Protokol::~Protokol()
{
}

void Protokol::unpacking(std::string s)
{
	std::string seqNum = s.substr(0, 3);
	std::string lastB = s.substr(4);
	std::string data = s.substr(4, 4 + dataSize);
	std::string check = s.substr(5 + dataSize, dataSize + 5 + 7);

	sequenceNumber = seqNum;
	lastBit = lastB;
	recievedData = data;
	checksum = check;
}

void Protokol::setLastBit()
{
	lastBit = endString.substr(4)
}
