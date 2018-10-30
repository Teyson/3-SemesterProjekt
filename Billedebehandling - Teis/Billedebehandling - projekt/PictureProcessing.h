#include "CImg.h"
#include <vector>
#include <bitset>

using namespace cimg_library;

#pragma once
class PictureProcessing
{
public:
	PictureProcessing();

	CImg<unsigned char> pictureResize(CImg<unsigned char> b);
	std::vector<std::vector<std::vector<int>>> RGBto8Bit(CImg<unsigned char> b);

	void colorValues2bit(std::vector<std::vector<std::vector<int>>> input);
	std::vector<std::vector<std::bitset<8>>> getcolourValues();
	int getR8bitColor();
	int getG8bitColor();
	int getB8bitColor();
	void byteTO8bitcolorvalues(std::bitset<8> byteInput);

	~PictureProcessing();
protected:
	std::vector<std::vector<std::bitset<8>>> colorValues;
	int r8bitColor;
	int g8bitColor;
	int b8bitColor;
};

