#include "customRecorder.h"
#include "Behandling.h"
#include <thread>


customRecorder::customRecorder()
{
}

bool customRecorder::onStart()
{
	// initialize whatever has to be done before the capture starts

	setProcessingInterval(sf::milliseconds(40)); //S�tter intervallerne som onProcessSamples() arbejder p�
	filter = 1;
												   // return true to start the capture, or false to cancel it
	return true;
}

bool customRecorder::onProcessSamples(const sf::Int16* samples, std::size_t sampleCount)
{
	

    testVector.clear();
    while (filter == 1)
	{
        Behandling test;
		for (int i = 0; i < sampleCount; i++)
		{
			testVector.push_back(samples[i]);
		}
        
        int amplitudeLow;
		int amplitudeHigh;
        int size = sampleCount / 4
		      

		for (size_t i = 0; i < 4; i++)
		{
            //std::cout << test.bestHigh(8000, &testVector, (i*size), size) << std::endl;
			amplitudeLow = test.goertzler(8000, 697, &testVector, (i*size), size);
			amplitudeHigh = test.goertzler(8000, 1209, &testVector, (i*size), size);
            //std::cout << amplitudeLow<< std::endl;
            int amplitudeLow1 = test.goertzler(8000, 941, &testVector, (i*size), size);
            std::cout << amplitudeLow << std::endl;
            std::cout << amplitudeHigh << std::endl;

            if (amplitudeHigh > 1000000 && amplitudeLow > 210010000){
                //std::cout << amplitudeLow << std::endl;
				if (i != 0) 
					i--;
                obj.clearMainBuffer(true);
                obj.addToMainBuffer(samples, i*size, sampleCount);
				filter = 0;
				break;
			}
            break;
		}
		return true;
	}
    
    obj.addToMainBuffer(samples, 0, sampleCount);

	
	// return true to continue the capture, or false to stop it
	return true;
}



void customRecorder::onStop()
{
	// clean up whatever has to be done after the capture is finished
	//...
}


void customRecorder::startThread()
{
	
	obj.startThread();
}


customRecorder::~customRecorder()
{
}
