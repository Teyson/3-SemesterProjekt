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

	//const sf::Int16 * getVector(int k);

	int getVectorSize();

	//const sf::Int16* returnBuffer();


	~customRecorder();

protected:
	//vector <double> sampleVector;
	//vector <const sf::Int16*> sampleVector;
	vector <int> sampleVector;
	//int sampleArray[5000];
	//sf::SoundBuffer buffer;
	//const sf::Int16* testBit;

};