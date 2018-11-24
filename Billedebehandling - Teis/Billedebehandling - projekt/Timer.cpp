#include "Timer.h"



Timer::Timer()
{
}

/*bool Timer::getrunning() {
	return running;
}*/

void Timer::test()
{
	std::atomic<bool> running = false;
	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
		while (running) {
			std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
			std::chrono::duration<double> differens = end - start;
			if (differens.count() > 0.5)
				running = false;
			running = true;
			std::this_thread::sleep_for(std::chrono::milliseconds(15));
		}
}

Timer::~Timer()
{
}
