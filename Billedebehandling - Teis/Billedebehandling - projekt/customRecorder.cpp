#include "customRecorder.h"


customRecorder::customRecorder()
{

}

bool customRecorder::onStart()
{
	// initialize whatever has to be done before the capture starts

	setProcessingInterval(sf::milliseconds(5)); //Sætter intervallerne som onProcessSamples() arbejder på


												   // return true to start the capture, or false to cancel it
	return true;
}

bool customRecorder::onProcessSamples(const sf::Int16* samples, std::size_t sampleCount)
{
	for (int i = 0; i < sampleCount; i++)
		{
			
			sampleVector.push_back(samples[i]);
			//cout << samples[i];
		}
	
	return false;
	// do something useful with the new chunk of samples
	//...
	//sf::SoundBuffer buffer;	
	//buffer.loadFromSamples(samples, sampleCount, 1, 44100);

	//cout <<  " BUFFER: " << buffer.getSamples() << endl;
	//cout << " SAMPLE: " << samples << endl;

	/*float maxSample = 0;
	for (int i = 0; i < sampleCount; i++)
	{
	maxSample = abs(samples[i]);
	}*/


	

	/*for (int i = 0; i < sampleCount; i++)
	{
	cout << sampleVector[i] << endl;
	}*/


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

//const sf::Int16 * customRecorder::getVector(int k)
//{
//	return sampleVector[k];
//}
//
//int customRecorder::getVectorSize()
//{
//	return testVector.size();
//
//}
//
//const sf::Int16* customRecorder::returnBuffer()
//{	return buffer.getSamples();
//}



customRecorder::~customRecorder()
{
}
