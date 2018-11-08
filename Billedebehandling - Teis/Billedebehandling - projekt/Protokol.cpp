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

std::string Protokol::getString()
{
	return endString;
}

Protokol::~Protokol()
{
}
