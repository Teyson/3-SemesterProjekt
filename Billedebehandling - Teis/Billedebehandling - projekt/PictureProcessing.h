#include "CImg.h"
#include <vector>
#include <bitset>

using namespace cimg_library;

#pragma once
class PictureProcessing
{
public:
	PictureProcessing();

	CImg<unsigned char> pictureResize(CImg<unsigned char> b);		//Tager et billede som input, returnerer et billede der er resized til 40x40
	std::vector<std::vector<std::vector<int>>> RGBto8Bit(CImg<unsigned char> b);		//Tager et billede som input, returnerer en vektortabel med 8Bit-farveværdier liggende på pixelens plads

	void colorValues2bit(std::vector<std::vector<std::vector<int>>> input);		//Tager vektortabellen med 8bit-farveværdier, laver dem om til bit strenge og gemmer dem i klassevariablen "colorValues"
	std::vector<std::vector<std::bitset<8>>> getcolourValues();		//Simpel get-metode der returnerer tabellen med bitsets
	void byteTO8bitcolorvalues(std::bitset<8> byteInput);		//modtagersidens konverterings-metode. Tager et bitset som input, omformaterer det til 8bit-farve-værdier og gemmer det på en position i klassevariablen "recievedColorValuesVector"
	void color8BitToRGB();		//Iterer igennem "recievedColorValuesVector" og laver det om til RGB-værdier og lægger dem i klassevariablen "finalValuesVector"
	CImg<unsigned char> finalPicture();		//Iterer igennem finalValuesVector", og returnerer udfra det et billede der repræsenterer de modtagede værdier

	~PictureProcessing();
protected:
	std::vector<std::vector<std::bitset<8>>> colorValues;
	std::vector<std::vector<std::vector<int>>> finalValuesVector;
	std::vector<std::vector<std::vector<int>>> recievedColorValuesVector;
	int finalValueVectorIndex_Y = 0;
	int finalValueVectorIndex_X = 0;
	int recievedColorValuesVectorIndex_Y = 0;
	int recievedColorValuesVectorIndex_X = 0;
};

