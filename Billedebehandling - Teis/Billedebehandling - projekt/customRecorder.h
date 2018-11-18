#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <array>

//#include <iostream>


#pragma once
class customRecorder : public sf::SoundRecorder
{
public:
	customRecorder();

	virtual bool onStart();

	virtual bool onProcessSamples(const sf::Int16* samples, std::size_t sampleCount);

	std::vector<sf::Int16> sampleToner();

	virtual void onStop();

	int getVector(int k);

	int getVectorSize();


	~customRecorder();

protected:
	//vector <double> sampleVector;
	//vector <const sf::Int16*> sampleVector;
	std::vector <int> sampleVector;
	//int sampleArray[5000];
	//sf::SoundBuffer buffer;
	//const sf::Int16* testBit;

};