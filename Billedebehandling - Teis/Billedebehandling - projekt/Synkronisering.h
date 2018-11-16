#pragma once
#include <vector>
#include <string>
#include <mutex>
#include <iostream>

class Synkronisering
{
public:
	Synkronisering();
    void startSync();


	~Synkronisering();

protected:
	std::vector<int> mainBuffer;
    std::vector<int> syncBuffer;
    

    int mainPointer;
    int syncPointer;

};

//Loader ind i vinduer

