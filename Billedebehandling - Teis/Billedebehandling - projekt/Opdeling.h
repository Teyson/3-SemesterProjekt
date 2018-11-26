#pragma once
#include <string>
#include <iostream>
#include <vector>


class Opdeler
{
public:
	Opdeler();
	Opdeler(std::string);

	std::vector<std::string> opdel(int opdelingsS);


	~Opdeler();
protected:
	std::string inputString;
	int opdelingsSize;

	std::vector<std::string> stringVector;

};

