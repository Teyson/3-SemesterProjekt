#include "Protokol.h"
#include <bitset>

Protokol::Protokol(std::string sr)
{
	if (sr.size() != dataSize + 16)
	{
		//Constructor til n�r vi modtager et NAK. Tager hele den modtagede NAK-sekvens som input.
		recievedNAK = sr;
	}
	else
	{
		//Constructor til n�r vi modtager en besked. Tager hele den modtagede bitstreng som input.
		recievedPacket = sr;
	}
	
}

Protokol::Protokol(std::string sr, int plac)
{
	//Constructor til n�r vi skal afsende data. Tager data bitstreng som input
	startString = sr;
	endString = startString;
	placering = plac;
}

void Protokol::unpacking()
{
	checksum = recievedPacket.substr(recievedPacket.length() - 8, 8);
	lastBit = recievedPacket.substr(4, 1);
	sequenceNumber = recievedPacket.substr(0, 4);
	data = recievedPacket.substr(8, dataSize);
}

std::string Protokol::header()
{
	int seqNum = placering % 15;		//Finder sekvensnummeret (decimalt) udfra det inputtede placeringsnummer.

	std::string seqNumString = std::bitset<4>(seqNum).to_string();

	std::string zeropadding = "000";

	std::string lastB = "0";

	std::string header = seqNumString + lastB + zeropadding;

	endString = header + endString;

	beforeTrailer = endString;

	return header;
}

std::string Protokol::trailer()
{
	CRC check(endString);

	std::string trailer = check.crcCheck();		//Se CRC-klassen for forklaring af crcCheck().

	endString = endString + trailer;
	return trailer;
}

std::string Protokol::packing()
{
	//Lavet for at g�re main() mindre. Begge �ndrer i endString, og det er altid vigtigt at trailer() kaldes EFTER header().
	//Derfor er de lagt sammen, for at det ikke kan g� galt.

	header();
	trailer();

	return endString;
}

void Protokol::setLastBit()
{
	//Deler endString op i dets delelementer, og �ndrer derefter lastBit, hvorefter den s�ttes sammen igen.
	std::string lastB = "1";

	std::string seqNum = beforeTrailer.substr(0, 4);
	std::string resendBit = beforeTrailer.substr(5, 1);
	std::string data = beforeTrailer.substr(8, dataSize);
	std::string zeropadding = "00";

	beforeTrailer = seqNum + lastB + resendBit + zeropadding + data;
	endString = beforeTrailer;
	trailer();
}

void Protokol::clearLastBit()
{
	//Deler endString op i dets delelementer, og �ndrer derefter lastBit, hvorefter den s�ttes sammen igen.

	std::string lastB = "0";

	std::string seqNum = beforeTrailer.substr(0, 4);
	std::string resendBit = beforeTrailer.substr(5, 1);
	std::string data = beforeTrailer.substr(8, dataSize);
	std::string zeropadding = "00";

	beforeTrailer = seqNum + lastB + resendBit + zeropadding + data;
	endString = beforeTrailer;
	trailer();
	endString;
}

std::string Protokol::getData()
{
	return endString.substr(8, 40);
}

std::string Protokol::getSequenceNumber()
{
	return endString.substr(0, 4);
}

void Protokol::setResendBit()
{
	//Deler endString op i dets delelementer, og �ndrer derefter lastBit, hvorefter den s�ttes sammen igen.

	std::string resendBit = "1";

	std::string seqNum = beforeTrailer.substr(0, 4);
	std::string lastB = beforeTrailer.substr(4, 1);
	std::string data = beforeTrailer.substr(8, dataSize);
	std::string zeropadding = "00";

	beforeTrailer = seqNum + lastB + resendBit + zeropadding + data;
	endString = beforeTrailer;
	trailer();
}

std::string Protokol::getCRCcheck()
{
	return endString.substr(endString.length() - 8, 8);
}

std::string Protokol::getLastBit()
{
	return endString.substr(4, 1);
}


bool Protokol::checkNAKChecksum()
{
	CRC check(recievedNAK);

	std::string checkedS = check.crcCheckReciever();		//Se CRC-klassen for forklaring af crcCheckReciever()

	int checkedSum = std::stoi(checkedS);

	if (checkedSum != 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

std::vector<std::string> Protokol::getNAKs()
{
	std::vector<std::string> NAKs;

	for (int i = 8; i < recievedNAK.size() - 8; i += 4)
	{
		NAKs.push_back(recievedNAK.substr(i, 4));
	}

	return NAKs;
}

bool Protokol::checkChecksum()
{
	CRC check(recievedPacket);
	
	std::string checkedS = check.crcCheckReciever();

	int checkedSum = std::stoi(checkedS);

	if (checkedSum != 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

std::string Protokol::writeChecksum()
{
	CRC check(recievedPacket);

	std::string checkedS = check.crcCheckReciever();

	return checkedS;
}

bool Protokol::checkLastBit()
{
	std::string lastB = getRecievedLastBit();

	if (lastB == "1")
	{
		return true;
	}
	else
	{
		return false;
	}

	return false;
}

std::string Protokol::getRecievedChecksum()
{
	return checksum;
}

std::string Protokol::getRecievedLastBit()
{
	
	return lastBit;
}

std::string Protokol::getRecievedSequenceNumber()
{
	return sequenceNumber;
}

std::string Protokol::getRecievedData()
{
	return data;
}

void Protokol::setToneStart(int s)
{
	toneNrStart = s;
}

void Protokol::setToneSlut(int s)
{
	toneNrSlut = s;
}

int Protokol::getToneStart()
{
	return toneNrStart;
}

int Protokol::getToneSlut()
{
	return toneNrSlut;
}

void Protokol::setDataSize(int dataS)
{
	dataSize = dataS;
}

std::string Protokol::getString()
{
	return endString;
}

std::string Protokol::getRecievedPacket()
{
	return recievedPacket;
}

std::string Protokol::getResendBit()
{
	return endString.substr(5, 1);
}

Protokol::~Protokol()
{
}
