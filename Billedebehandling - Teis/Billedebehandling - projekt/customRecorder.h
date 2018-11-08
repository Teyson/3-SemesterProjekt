#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <array>

//#include <iostream>
using namespace std;

#pragma once
class customRecorder : public sf::SoundRecorder
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
	vector <int> sampleVector;

};