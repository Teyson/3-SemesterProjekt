#include "TextProcessing.h"



TextProcessing::TextProcessing()
{
}

TextProcessing::TextProcessing(std::string inputStr)
{
	inputString = inputStr;
}

std::string TextProcessing::getInputString()
{
	return inputString;
}

void TextProcessing::setInputString(std::string s)
{
	inputString = s;
}

std::string TextProcessing::getOutputString()
{
	return outputString;
}

std::vector<std::bitset<8>> TextProcessing::stringToBitsVector()
{
	std::string newString = inputString;
	
	std::vector<std::bitset<8>> finalVector;

	for (int i = 0; i < newString.size(); i++)
	{
		finalVector.push_back(std::bitset<8>((int)newString[i]));
	}
	
	bitsetsVector = finalVector;

	return finalVector;
}

std::string TextProcessing::bitsetsVectorToString()
{
	std::string finalString;

	for (int i = 0; i < bitsetsVector.size(); i++)
	{
		std::bitset<8> bits = bitsetsVector[i];
		std::string bitString = bits.to_string();
		finalString.append(bitString);
	}

	outputString = finalString;
	return outputString;
}

std::string TextProcessing::stringToBitsString()
{
	stringToBitsVector();
	std::string returnString = bitsetsVectorToString();

	return returnString;
}

TextProcessing::~TextProcessing()
{
}
