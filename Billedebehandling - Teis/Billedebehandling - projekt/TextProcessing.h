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

	std::vector<std::bitset<8>> stringToBitsVector();
	std::string bitsetsVectorToString();
	std::string stringToBitsString();

	~TextProcessing();

protected:
	std::string inputString;
	std::string outputString;
	std::vector<std::bitset<8>> bitsetsVector;
};