#include "CImg.h"
#include <vector>

using namespace cimg_library;

#pragma once
class PictureProcessing
{
public:
	PictureProcessing();

	CImg<unsigned char> pictureResize(CImg<unsigned char> b);
	std::vector<std::vector<std::vector<int>>> RGBto8Bit(CImg<unsigned char> b);

	~PictureProcessing();
};

