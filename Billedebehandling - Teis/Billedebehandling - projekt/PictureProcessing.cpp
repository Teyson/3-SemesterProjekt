#include "PictureProcessing.h"

PictureProcessing::PictureProcessing()
{
}

CImg<unsigned char> PictureProcessing::pictureResize(CImg<unsigned char> b)
{
	CImg<unsigned char> bResized = b.resize(40, 40);
	
	return bResized;
}

std::vector<std::vector<std::vector<int>>> PictureProcessing::RGBto8Bit(CImg<unsigned char> b)
{
	CImg<unsigned char> image = b;

	std::vector<std::vector<std::vector<int>>> pixelValues8Bit(image.height(),std::vector<std::vector<int>>(image.width(),std::vector<int>(3,0)));

	for (int i = 1; i < image.height(); i++)
	{
		for (int j = 1; j < image.width(); j++)
		{
			unsigned char* r = image.data(j, i, 0);
			unsigned char* g = image.data(j, i, 1);
			unsigned char* b = image.data(j, i, 2);

			float rFloat = *r;
			float gFloat = *g;
			float bFloat = *b;

			int newRValue = floor((rFloat / 256) * 8);
			int newGValue = floor((gFloat / 256) * 8);
			int newBValue = floor((bFloat / 256) * 4);

			pixelValues8Bit[j][i][0] = newRValue;
			pixelValues8Bit[j][i][1] = newGValue;
			pixelValues8Bit[j][i][2] = newBValue;
		}
	}
	return pixelValues8Bit;
}

void PictureProcessing::colorValues2bit(std::vector<std::vector<std::vector<int>>> input)
{
	for (int i = 0; i <= 40; i++) {
		for (int k = 0; k <= 40; k++) {
			std::bitset<3>	rBit3(input[k][i][0]);
			std::bitset<3>	gBit3(input[k][i][1]);
			std::bitset<2>	bBit2(input[k][i][2]);

			std::string stringByte = rBit3.to_string() + gBit3.to_string() + bBit2.to_string();

			std::bitset<8> colorByte(stringByte);


			colorValues[k][i] = colorByte;

		}
	}

}

std::vector<std::vector<std::bitset<8>>> PictureProcessing::getcolourValues()
{
	return colorValues;
}

void PictureProcessing::byteTO8bitcolorvalues(std::bitset<8> byteInput)
{
	std::bitset<3> bitR(byteInput.to_string().substr(0, 3));
	std::bitset<3> bitG(byteInput.to_string().substr(3, 3));
	std::bitset<2> bitB(byteInput.to_string().substr(7, 2));

	int r8bitColor = (int)(bitR.to_ulong());
	int g8bitColor = (int)(bitG.to_ulong());
	int b8bitColor = (int)(bitB.to_ulong());

	recievedColorValuesVector[recievedColorValuesVectorIndex_X][recievedColorValuesVectorIndex_Y][0] = r8bitColor;
	recievedColorValuesVector[recievedColorValuesVectorIndex_X][recievedColorValuesVectorIndex_Y][1] = g8bitColor;
	recievedColorValuesVector[recievedColorValuesVectorIndex_X][recievedColorValuesVectorIndex_Y][2] = b8bitColor;

	if (recievedColorValuesVectorIndex_X == 40)
	{
		recievedColorValuesVectorIndex_X = 0;
		recievedColorValuesVectorIndex_Y++;
	}
	else
	{
		recievedColorValuesVectorIndex_X++;
	}
}

void PictureProcessing::color8BitToRGB()
{
	for (int i = 1; i < recievedColorValuesVector.size(); i++)
	{
		for (int j = 1; j < recievedColorValuesVector[i].size(); j++)
		{
			float rFloat = recievedColorValuesVector[j][i][0];
			float gFloat = recievedColorValuesVector[j][i][1];
			float bFloat = recievedColorValuesVector[j][i][2];

			int newRValue = (rFloat / 8) * 256;
			int newGValue = (gFloat / 8) * 256;
			int newBValue = (bFloat / 4) * 256;

			finalValuesVector[j][i][0] = newRValue;
			finalValuesVector[j][i][1] = newGValue;
			finalValuesVector[j][i][2] = newBValue;
		}
	}
}

CImg<unsigned char> PictureProcessing::finalPicture()
{
	CImg<unsigned char> finalPicture;
	
	for (int i = 1; i < finalValuesVector.size(); i++)
	{
		for (int j = 1; j < finalValuesVector[i].size(); j++)
		{
			finalPicture(j, i, 0) = finalValuesVector[j][i][0];
			finalPicture(j, i, 1) = finalValuesVector[j][i][1];
			finalPicture(j, i, 2) = finalValuesVector[j][i][2];
		}
	}
	return finalPicture;
}

PictureProcessing::~PictureProcessing()
{
}
