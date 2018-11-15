#include "customRecorder.h"
#include "Behandling.h"


customRecorder::customRecorder()
{

}

bool customRecorder::onStart()
{
	// initialize whatever has to be done before the capture starts

	setProcessingInterval(sf::milliseconds(100)); //S�tter intervallerne som onProcessSamples() arbejder p�
	
												   // return true to start the capture, or false to cancel it
	return true;
}

bool customRecorder::onProcessSamples(const sf::Int16* samples, std::size_t sampleCount)
{
	
	
	mainBuffer;


	std::cout << sampleCount << std::endl;
	//	sampleVector.clear();
	//std::cout << sampleCount << std::endl;
	for (int i = 0; i < sampleCount; i++)
		{	
		sampleVector.push_back(samples[i]);
		//std::cout << sampleVector[i] << std::endl;
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
	return sampleVector[k];
}

int customRecorder::getVectorSize()
{
	return sampleVector.size();

}





customRecorder::~customRecorder()
{
}
