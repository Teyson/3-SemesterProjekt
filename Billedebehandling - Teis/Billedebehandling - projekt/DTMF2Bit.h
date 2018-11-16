#pragma once
#include <string>
#include <vector>
#include "Behandling.h"


class DTMF2Bit
{
public:
	DTMF2Bit();
	std::string convertDTMF2Nibble(int fs, std::vector<int> sampels);
	~DTMF2Bit();
};

