#include "customRecorder.h"
#include "Behandling.h"


customRecorder::customRecorder()
{

}

bool customRecorder::onStart()
{
	// initialize whatever has to be done before the capture starts

	setProcessingInterval(sf::milliseconds(20)); //Sætter intervallerne som onProcessSamples() arbejder på
	startPointer = 0;
	
	filter = 0;
												   // return true to start the capture, or false to cancel it
	return true;
}

bool customRecorder::onProcessSamples(const sf::Int16* samples, std::size_t sampleCount)
{
	/*while (filter == 0)
	{
		std::cout << "filter" << endl;
		for (int i = 0; i < sampleCount; i + 100)
		{
			for (int j = 0; j < 100; j++)
			{
				testVector.push_back(samples[j]);
			}

			Behandling blbla;
			if (blbla.goertzler(8000, 697, testVector) > 10000)
			{
				filter = 1;
				return true;
			}
		}
		break;
	}*/
	for (int i = 0; i < sampleCount; i++)
	{
		sampleVector.push_back(samples[i]);
		//std::cout << sampleVector[i] << std::endl;
	}
	

	Behandling blabla;
	std::cout << blabla.goertzler(8000, 697, &sampleVector, startPointer, sampleCount) << std::endl;

	sampleVector.clear();

	
	//mainBuffer;


	//std::cout << sampleCount << std::endl;
	//	sampleVector.clear();
	//std::cout << sampleCount << std::endl;
	//for (int i = 0; i < sampleCount; i++)
	//	{	
	//	sampleVector.push_back(samples[i]);
	//	//std::cout << sampleVector[i] << std::endl;
	//	}


	

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
	return sampleVector[k];
}

int customRecorder::getVectorSize()
{
	return sampleVector.size();

}





customRecorder::~customRecorder()
{
}
