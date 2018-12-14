#include "Protokol.h"


Protokol::Protokol()
{
}

Protokol::Protokol(std::string s)
{
	startString = s;
	endString = startString;
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
	std::cout << toneNrSlut << std::endl;
	return toneNrSlut;
}

std::string Protokol::getString()
{
	return endString;
}

Protokol::~Protokol()
{
}
