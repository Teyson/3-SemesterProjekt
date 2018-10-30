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


PictureProcessing::~PictureProcessing()
{
}
