#include "customRecorder.h"
#include "Behandling.h"


customRecorder::customRecorder()
{
}

bool customRecorder::onStart()
{
	// initialize whatever has to be done before the capture starts

	setProcessingInterval(sf::milliseconds(50)); //S�tter intervallerne som onProcessSamples() arbejder p�
	
	filter = 0;
												   // return true to start the capture, or false to cancel it
	return true;
}

bool customRecorder::onProcessSamples(const sf::Int16* samples, std::size_t sampleCount)
{
	while (filter == 0)
	{
		testVector.clear();
		for (int i = 0; i < sampleCount; i++)
		{
			testVector.push_back(samples[i]);
		}

		int amplitudeLow;
		int amplitudeHigh;
		int size = sampleCount / 10;
		Behandling test;

		for (size_t i = 0; i < 10; i++)
		{
			amplitudeLow = test.goertzler(8000, 697, &testVector, (i*size), size);
			amplitudeHigh = test.goertzler(8000, 1209, &testVector, (i*size), size);
			

			if (amplitudeHigh > 1500 && amplitudeLow > 1100){
				if (i != 0) 
					i--;
				for (int j = i*size; j < 10*size; j++)
				{
					mainBuffer.push_back(testVector[j]);
				}
				filter = 1;
				return true;
			}
		}
		return true;
	}

	for (int i = 0; i < sampleCount; i++)
	{
		mainBuffer.push_back(samples[i]);
	}

	

	Behandling komnumand;

	//std::cout << komnumand.bestHigh(8000, sampleVector) << " + " << komnumand.bestLow(8000, sampleVector) << std::endl;

	
	// return true to continue the capture, or false to stop it
	return true;
}


void customRecorder::onStop()
{
	// clean up whatever has to be done after the capture is finished
	//...
}

int customRecorder::getVector(int k)
{
	return mainBuffer[k];
}

int customRecorder::getVectorSize()
{
	return mainBuffer.size();

}





customRecorder::~customRecorder()
{
}
