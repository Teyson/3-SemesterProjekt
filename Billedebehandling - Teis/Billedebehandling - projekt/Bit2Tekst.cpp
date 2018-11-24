#include "Bit2Tekst.h"



Bit2Tekst::Bit2Tekst(std::string s)
{
	data = s;
}

std::string Bit2Tekst::bitToString()
{
	for (int i = 0; i < data.length() / 8; i++)
	{
		char bToS = static_cast<char>(std::stoi(data.substr(i * 8, 8), nullptr, 2));
		slutString += bToS;
	}
	
	return slutString;
}

Bit2Tekst::~Bit2Tekst()
{
}
