#include <iostream>
#include "CImg.h"
#include "PictureProcessing.h"

using namespace cimg_library;

int main()
{
	PictureProcessing Behandling;

	CImg<unsigned char> image("beavis.bmp"), visu(500,400,1,3,0);
	CImg<unsigned char> imageCopy("beavis.bmp");



	const unsigned char red[] = { 255,0,0 }, green[] = { 0,255,0 }, blue[] = { 0,0,255 };
	image.resize(40,40);
	imageCopy.resize(40, 40);
	image.resize(100,100);
	imageCopy.resize(100, 100);
	for (int i = 1; i < image.height(); i++)
	{
		for (int j = 1; j < image.width(); j++)
		{
			unsigned char* r = image.data(j, i, 0);
			unsigned char* g = image.data(j, i, 1);
			unsigned char* b = image.data(j, i, 2);

			float rInt = *r;
			float gInt = *g;
			float bInt = *b;

			float newRValue1 = floor((rInt / 256) * 8);
			float newGValue1 = floor((gInt / 256) * 8);
			float newBValue1 = floor((bInt / 256) * 4);

			int newRValue = (newRValue1 / 8) * 256;
			int newGValue = (newGValue1 / 8) * 256;
			int newBValue = (newBValue1 / 4) * 256;

			imageCopy(j, i, 0) = newRValue;
			imageCopy(j, i, 1) = newGValue;
			imageCopy(j, i, 2) = newBValue;
		}
	}
	CImgDisplay main_disp(image, "Click a point"), second_disp(imageCopy, "this is the result"), draw_disp(visu, "Intensity profile");
	while (!main_disp.is_closed() && !draw_disp.is_closed()) {
		main_disp.wait();
		if (main_disp.button() && main_disp.mouse_y() >= 0) {
			const int y = main_disp.mouse_y();
			visu.fill(0).draw_graph(image.get_crop(0, y, 0, 0, image.width() - 1, y, 0, 0), red, 1, 1, 0, 255, 0);
			visu.draw_graph(image.get_crop(0, y, 0, 1, image.width() - 1, y, 0, 1), green, 1, 1, 0, 255, 0);
			visu.draw_graph(image.get_crop(0, y, 0, 2, image.width() - 1, y, 0, 2), blue, 1, 1, 0, 255, 0).display(draw_disp);
		}
	}
	return 0;
}