#include <iostream>
#include "CImg.h"
#include "PictureProcessing.h"
#include <cmath>
#include <iostream>
#include "customRecorder.h"

int main() {
	const unsigned SAMPLES = 44100;
	const unsigned SAMPLE_RATE = 44100;
	const unsigned AMPLITUDE = 10000;

	sf::Int16 raw[SAMPLES];

	const double TWO_PI = 6.28318;
	const double increment1 = 697. / 44100;
	const double increment2 = 1209. / 44100;
	double x1 = 0;
	double x2 = 0;
	for (unsigned i = 0; i < SAMPLES; i++) {
		raw[i] = AMPLITUDE * (sin(x1*TWO_PI) + sin(x2*TWO_PI));
		x1 += increment1;
		x2 += increment2;


	}

	sf::SoundBuffer Buffer;
	if (!Buffer.loadFromSamples(raw, SAMPLES, 1, SAMPLE_RATE)) {
		std::cerr << "Loading failed!" << std::endl;
		return 1;
	}

	sf::Sound Sound;
	Sound.setBuffer(Buffer);
	Sound.setLoop(true);
	Sound.play();
	while (1) {
		sf::sleep(sf::milliseconds(100));
	}
	return 0;
}