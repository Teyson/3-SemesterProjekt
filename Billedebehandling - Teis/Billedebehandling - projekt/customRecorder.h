#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <array>

#include "Synkronisering.h"


using namespace std;

#pragma once
class customRecorder : public sf::SoundRecorder, Synkronisering
{
public:
	customRecorder();

	virtual bool onStart();

	virtual bool onProcessSamples(const sf::Int16* samples, std::size_t sampleCount);

	virtual void onStop();

	int getVector(int k);

	int getVectorSize();


	~customRecorder();

protected:
	std::vector <int> sampleVector;
	std::vector <int> testVector;
	int counter;
	int filter;
	int startPointer;
};