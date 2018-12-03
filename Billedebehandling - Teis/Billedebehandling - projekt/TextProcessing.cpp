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

std::string TextProcessing::stringToBitsString()
{
	std::string newString = inputString;

	std::string finalString = "";

	for (int i = 0; i < newString.size(); i++)
	{
		std::string bitsString = std::bitset<8>((int)newString[i]).to_string();
		finalString.append(bitsString);
	}

	outputString = finalString;

	return finalString;
}

TextProcessing::~TextProcessing()
{
}
