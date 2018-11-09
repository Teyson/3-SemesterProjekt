#pragma once
#ifndef PROTOKOL
#define PROTOKOL

#include <string>
#include <vector>
//#include "BitDTMF.h" SKAL IKKE INKLUDERES


class Protokol
{
public:
	Protokol();
	Protokol(std::string);
	//Protokol - Datadelen til protokollen

	void setToneStart(int);
	void setToneSlut(int);
	int getToneStart();
	int getToneSlut();
	std::string getString();
	~Protokol();

protected:
	std::string startString;
	std::string endString;
	int toneNrStart;
	int toneNrSlut;
};

#endif

