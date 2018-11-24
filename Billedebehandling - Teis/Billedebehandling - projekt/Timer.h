#pragma once
#include<thread>
#include<chrono>
#include <atomic>

class Timer
{
public:
	Timer();
	void test();
	//bool getrunning();
	~Timer();
protected:
	//bool running;
	int f;
};

