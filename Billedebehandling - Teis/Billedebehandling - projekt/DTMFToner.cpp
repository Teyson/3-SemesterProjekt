#include "DTMFToner.h"



DTMFToner::DTMFToner()
{
}

DTMFToner::DTMFToner(std::string s)
{
	startString = s;
}

int DTMFToner::createDtmf()
{
	if (startString == "0001")
		tone = 1;
	if (startString == "0010")
		tone = 2;
	if (startString == "0011")
		tone = 3;
	if (startString == "0100")
		tone = 4;
	if (startString == "0101")
		tone = 5;
	if (startString == "0110")
		tone = 6;
	if (startString == "0111")
		tone = 7;
	if (startString == "1000")
		tone = 8;
	if (startString == "1001")
		tone = 9;
	if (startString == "1010")
		tone = 10;
	if (startString == "1011")
		tone = 11;
	if (startString == "1100")
		tone = 12;
	if (startString == "1101")
		tone = 13;
	if (startString == "1110")
		tone = 14;
	if (startString == "1111")
		tone = 15;


	return tone;
}


DTMFToner::~DTMFToner()
{
}
