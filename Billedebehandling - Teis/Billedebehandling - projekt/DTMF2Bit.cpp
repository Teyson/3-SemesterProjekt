#include "DTMF2Bit.h"



DTMF2Bit::DTMF2Bit()
{
}

std::string DTMF2Bit::convertDTMF2Nibble(int fs, std::vector<int> sampels) {
	Behandling b;
	int high = b.bestHigh(fs, sampels);
	int low = b.bestLow(fs, sampels);
	std::string out;
	switch (low + high)
	{
	case 697 + 1209:
		out="0000";
		break;
	case 697 + 1336:
		out = "0001";
		break;
	case 697 + 1477:
		out = "0010";
		break;
	case 697+1633:
		out = "0011";
		break;
	case 770+1209:
		out = "0100";
		break;
	case 770+1336:
		out = "0101";
		break;
	case 770+1477:
		out = "0110";
		break;
	case 770+1633:
		out = "0111";
		break;
	case 852 + 1209:
		out = "1000";
		break;
	case 852+1336:
		out = "1001";
		break;
	case 852+1477:
		out = "1010";
		break;
	case 852+1633:
		out = "1011";
		break;
	case 941+1209:
		out = "1100";
		break;
	case 941+1336:
		out = "1101";
		break;
	case 941+1477:
		out = "1110";
		break;
	case 941+1633:
		out = "1111";
		break;
	default:
		out = "Err";
		break;
	}
	return out;
}

DTMF2Bit::~DTMF2Bit()
{
}
