#include "customRecorder.h"
#include "Behandling.h"


customRecorder::customRecorder()
{
	a = 0;
}

bool customRecorder::onStart()
{
	// initialize whatever has to be done before the capture starts

	setProcessingInterval(sf::milliseconds(50)); //Sætter intervallerne som onProcessSamples() arbejder på
	startPointer = 0;
	
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
		int size = sampleCount / 5;
		Behandling test;

		for (size_t i = 1; i < 6; i++)
		{
			amplitudeLow = test.goertzler(8000, 697, &testVector, (i*size) - size, size);
			amplitudeHigh = test.goertzler(8000, 1209, &testVector, (i*size) - size, size);
			

			if (amplitudeHigh > 2500 && amplitudeLow > 4000){
				filter = 1;
				return true;
			}
		}
		return true;
	}

	std::cout << "YOU DID IT BABY" << std::endl;


	//for (int i = 0; i < sampleCount; i++)
	//{
	//	sampleVector.push_back(samples[i]);
	//	//std::cout << sampleVector[i] << std::endl;
	//}
	//
	//Behandling blabla;
	////Vindue på 50/10 = 5ms
	//

	//for (size_t i = 1; i < 4; i++)
	//{
	//	//std::cout << blabla.goertzler(8000, 697, &sampleVector, (i*sampleCount / 3) - sampleCount / 3, sampleCount / 3) << std::endl;
	//	std::cout << blabla.goertzler(8000, 1209, &sampleVector, (i*sampleCount / 3) - sampleCount /3 , sampleCount / 3) << std::endl;
	//}
	//
	//

	//sampleVector.clear();

	//
	////mainBuffer;


	////std::cout << sampleCount << std::endl;
	////	sampleVector.clear();
	////std::cout << sampleCount << std::endl;
	////for (int i = 0; i < sampleCount; i++)
	////	{	
	////	sampleVector.push_back(samples[i]);
	////	//std::cout << sampleVector[i] << std::endl;
	////	}


	

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
