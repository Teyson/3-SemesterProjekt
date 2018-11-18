#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <array>
#include <fstream>
#include <mutex>;

#include "Synkronisering.h"

using namespace std;

#pragma once
class customRecorder : public sf::SoundRecorder
{
public:
	customRecorder();
	virtual bool onStart();
	virtual bool onProcessSamples(const sf::Int16* samples, std::size_t sampleCount);
	virtual void onStop();
    void synkronisering(int sampleRate, int vindueSizems);
	int getVector(int k);
	int getVectorSize();
    
    
	~customRecorder();

protected:
	std::vector <int> testVector;
    std::vector<int> mainBuffer;
	int filter;
    int mainPointer;
    int syncPointer;
    Synkronisering obj;

};