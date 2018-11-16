#pragma once
#include <vector>
#include <math.h>
#include <string>
#include <iostream>
class Behandling
{
public:
	Behandling();
	int goertzler(int fs, int f, std::vector<int>*samples, int, int);
	int bestLow(int fs, std::vector<int>*samples, int, int);
	int bestHigh(int fs, std::vector<int>*samples, int, int);
	~Behandling();
protected:

};

