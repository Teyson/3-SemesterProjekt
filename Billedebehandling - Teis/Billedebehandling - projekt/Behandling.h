#pragma once
#include <vector>
#include <math.h>
#include <string>
class Behandling
{
public:
	Behandling();
	int goertzler(int fs, int f, std::vector<int> sampels);
	int bestLowDTMF(int fs, std::vector<int>sampels);
	int bestHighDTMF(int fs, std::vector<int>sampels);
	~Behandling();
protected:
	int samplingFrekvens;
	std::vector<int> maalinger;
};

