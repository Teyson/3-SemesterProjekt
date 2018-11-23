#pragma once
#ifndef DTMFTONER
#define DTMFTONER

#include <vector>
#include <string>
#include <iostream>

class DTMFToner
{
public:
	DTMFToner();

	DTMFToner(std::string);

	int getToneNumber();

	void setToneNumber();
	//createDTMF - outputter hvilken tone strengen svarer til

	std::vector<float> createTone(int samples, int sampleRate, float lastVal, float secondLastVal);

	~DTMFToner();

protected:
	std::string startString;
	int tone;
    float pi;
	float twoPi;
	int amplitude;
	
};

#endif