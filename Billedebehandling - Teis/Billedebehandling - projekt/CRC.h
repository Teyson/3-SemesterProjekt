#pragma once
#include <string>
#include <iostream>

class CRC
{
public:
	CRC(std::string st);
	
	std::string getbitstreng();
	
	std::string crcCheck();

	std::string crcCheckReciever();
	

	~CRC();
protected:
	std::string bitStreng;
	std::string bSMCheckC;
	std::string divident = "11011";
};


