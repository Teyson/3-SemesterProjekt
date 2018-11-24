#pragma once
#include <string>
#include <bitset>
#include <vector>


class Bit2Tekst
{
public:
	Bit2Tekst(std::string s);
	std::string bitToString();
	~Bit2Tekst();

protected:
	std::string data;
	std::string slutString = "";

};

