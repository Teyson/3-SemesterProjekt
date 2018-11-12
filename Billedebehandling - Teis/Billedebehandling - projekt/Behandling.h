#pragma once
#include <vector>
#include <math.h>
#include <string>
#include <iostream>
class Behandling
{
public:
	Behandling();
	int goertzler(int fs, int f, std::vector<int> sampels);
	int bestLow(int fs, std::vector<int>sampels);
	int bestHigh(int fs, std::vector<int>sampels);

	void getLowMag();
	void getHighMag();
	~Behandling();
protected:
	int samplingFrekvens;
	std::vector<int> maalinger;
	int lowMagnitude[4];
	int highMagnitude[4];
};

