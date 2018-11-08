#pragma once
#include <vector>
#include <math.h>
#include <string>
class Behandling
{
public:
	Behandling();
	int goertzler(int fs, int f, std::vector<float> sampels);
	int bestLowDTMF(int fs, std::vector<float>sampels);
	int bestHighDTMF(int fs, std::vector<float>sampels);
	~Behandling();
protected:
	int samplingFrekvens;
	std::vector<float> maalinger;
};

