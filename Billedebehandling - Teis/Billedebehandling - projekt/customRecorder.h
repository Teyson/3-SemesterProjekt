#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <array>
#include <fstream>
#include <mutex>;

#include "Synkronisering.h"



#pragma once
class customRecorder : public sf::SoundRecorder
{
public:
	customRecorder();
	virtual bool onStart();
	virtual bool onProcessSamples(const sf::Int16* samples, std::size_t sampleCount);
	virtual void onStop();
	std::string startThread();
    
    
	~customRecorder();

protected:
	std::vector <int> testVector;
    std::vector<int> mainBuffer;
	int filter;
    int mainPointer;
    Synkronisering obj;
};