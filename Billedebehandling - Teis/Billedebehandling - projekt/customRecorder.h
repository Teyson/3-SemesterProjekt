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
	std::vector <sf::Int16> sampleVector;

};