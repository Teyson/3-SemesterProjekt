#pragma once
#include <vector>
#include <string>
#include <mutex>
#include <iostream>
#include <SFML/Audio.hpp>

class Synkronisering 
{
public:
	Synkronisering();
    void addToMainBuffer(const sf::Int16 *samples, int startPtr, int vectorSize);
    void clearMainBuffer(bool clear);
    void startSync();
    void setClass();

	~Synkronisering();

protected:
	//std::vector<int> mainBuffer;
    std::vector<int> syncBuffer;
    std::vector<int> mainBuffer;
    

    int a;
    int mainPtr;
    int syncPtr;

};

//Loader ind i vinduer

