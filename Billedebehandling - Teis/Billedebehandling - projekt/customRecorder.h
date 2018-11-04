#include <SFML/Audio.hpp>

#pragma once
class customRecorder : public sf::SoundRecorder
{
public:
	customRecorder();

	virtual bool onStart()
	{
		// initialize whatever has to be done before the capture starts

		setProcessingInterval(sf::milliseconds(5)); //Sætter intervallerne som onProcessSamples() arbejder på

		// return true to start the capture, or false to cancel it
		return true;
	}

	virtual bool onProcessSamples(const sf::Int16* samples, std::size_t sampleCount)
	{
		// do something useful with the new chunk of samples
		//...

		// return true to continue the capture, or false to stop it
		return true;
	}

	virtual void onStop() 
	{
		// clean up whatever has to be done after the capture is finished
		//...
	}

	~customRecorder();

protected:
};

