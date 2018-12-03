#include <string>
#include <vector>
#include <bitset>

#pragma once
class TextProcessing
{
public:
	TextProcessing();
	TextProcessing(std::string inputStr);

	std::string getInputString();
	void setInputString(std::string s);
	std::string getOutputString();

	std::string stringToBitsString();

	~TextProcessing();

protected:
	std::string inputString;
	std::string outputString;
};