#pragma once
#ifndef DTMFTONER
#define DTMFTONER

#include <vector>
#include <string>

class DTMFToner
{
public:
	DTMFToner();

	DTMFToner(std::string);


	int createDtmf();
	//createDTMF - outputter hvilken tone strengen svarer til

	~DTMFToner();

protected:
	std::string startString;
	int tone;
};

#endif