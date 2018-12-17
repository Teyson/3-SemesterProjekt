#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <array>
#include <fstream>
#include <mutex>;

#include "Synkronisering.h"



#pragma once
class CustomRecorder : public sf::SoundRecorder
{
public:
	CustomRecorder();
	virtual bool onStart();
	virtual bool onProcessSamples(const sf::Int16* samples, std::size_t sampleCount);
	virtual void onStop();
	std::string startThread();
    
    
	~CustomRecorder();

protected:
	std::vector <int> testVector;
    std::vector<int> mainBuffer;
	int filter;
    int mainPointer;
    Synkronisering obj;
};