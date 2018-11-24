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

	std::vector<float> createTone(int samples, int sampleRate);

	~DTMFToner();

protected:
	std::string startString;
	int tone;
	float twoPi;
	int amplitude;
	
};

#endif