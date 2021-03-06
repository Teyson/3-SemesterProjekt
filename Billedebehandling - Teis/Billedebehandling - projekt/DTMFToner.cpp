#include "DTMFToner.h"

DTMFToner::DTMFToner()
{
}

DTMFToner::DTMFToner(std::string s)
{
	startString = s;
	twoPi = 6.28318;
}

int DTMFToner::getToneNumber()
{
	return tone;
}

void DTMFToner::setToneNumber()
{
	if (startString == "0000")				// 697 & 1209
		tone = 0; amplitude = 10000;
	if (startString == "0001")				// 697 & 1336
		tone = 1; amplitude = 10000;
	if (startString == "0010")				// 697 & 1477
		tone = 2; amplitude = 10000;
	if (startString == "0011")				// 697 & 1633
		tone = 3; amplitude = 10000;
	if (startString == "0100")				// 770 & 1209
		tone = 4; amplitude = 10000;
	if (startString == "0101")				// 770 & 1336
		tone = 5; amplitude = 10000;
	if (startString == "0110")				// 770 & 1477
		tone = 6; amplitude = 10000;
	if (startString == "0111")				// 770 & 1633
		tone = 7; amplitude = 10000;
	if (startString == "1000")				// 852 & 1209
		tone = 8; amplitude = 10000;
	if (startString == "1001")				// 852 & 1336
		tone = 9; amplitude = 10000;
	if (startString == "1010")				// 852 & 1477
		tone = 10; amplitude = 10000;
	if (startString == "1011")				// 852 & 1633
		tone = 11; amplitude = 10000;
	if (startString == "1100")				// 941 & 1209
		tone = 12; amplitude = 10000;		
	if (startString == "1101")				// 941 & 1336
		tone = 13; amplitude = 10000;
	if (startString == "1110")				// 941 & 1477
		tone = 14; amplitude = 10000;
	if (startString == "1111")				// 941 & 1633
		tone = 15; amplitude = 10000;
}

std::vector<float> DTMFToner::createTone(int samples, int sampleRate)
{
	//const unsigned sampleRate = 8000;
	//const unsigned samples = 16000;

	const float increment1_1 = 697. / sampleRate;
	const float increment1_2 = 770. / sampleRate;
	const float increment1_3 = 852. / sampleRate;
	const float increment1_4 = 941. / sampleRate;

	const float increment2_1 = 1209. / sampleRate;
	const float increment2_2 = 1336. / sampleRate;
	const float increment2_3 = 1477. / sampleRate;
	const float increment2_4 = 1633. / sampleRate;

	//De to containere som skal holde de to frekvenser som DTMF-tonen skal best� af
	float x1 = 0;
	float x2 = 0;
	std::vector<float> toneVals;

	if (tone == 0) {
		for (unsigned i = 0; i < samples; i++) {
			toneVals.push_back(amplitude * (sin(x1*twoPi) + sin(x2*twoPi)));
			x1 += increment1_1;
			x2 += increment2_1;
		}
		return toneVals;
	}

	if (tone == 1) {
		for (unsigned i = 0; i < samples; i++) {
			toneVals.push_back(amplitude * (sin(x1*twoPi) + sin(x2*twoPi)));
			x1 += increment1_1;
			x2 += increment2_2;
		}
		return toneVals;
	}

	if (tone == 2) {
		for (unsigned i = 0; i < samples; i++) {
			toneVals.push_back(amplitude * (sin(x1*twoPi) + sin(x2*twoPi)));
			x1 += increment1_1;
			x2 += increment2_3;
		}
		return toneVals;
	}

	if (tone == 3) {
		for (unsigned i = 0; i < samples; i++) {
			toneVals.push_back(amplitude * (sin(x1*twoPi) + sin(x2*twoPi)));
			x1 += increment1_1;
			x2 += increment2_4;
		}
		return toneVals;
	}

	if (tone == 4) {
		for (unsigned i = 0; i < samples; i++) {
			toneVals.push_back(amplitude * (sin(x1*twoPi) + sin(x2*twoPi)));
			x1 += increment1_2;
			x2 += increment2_1;
		}
		return toneVals;
	}

	if (tone == 5) {
		for (unsigned i = 0; i < samples; i++) {
			toneVals.push_back(amplitude * (sin(x1*twoPi) + sin(x2*twoPi)));
			x1 += increment1_2;
			x2 += increment2_2;
		}
		return toneVals;
	}

	if (tone == 6) {
		for (unsigned i = 0; i < samples; i++) {
			toneVals.push_back(amplitude * (sin(x1*twoPi) + sin(x2*twoPi)));
			x1 += increment1_2;
			x2 += increment2_3;
		}
		return toneVals;
	}

	if (tone == 7) {
		for (unsigned i = 0; i < samples; i++) {
			toneVals.push_back(amplitude * (sin(x1*twoPi) + sin(x2*twoPi)));
			x1 += increment1_2;
			x2 += increment2_4;
		}
		return toneVals;
	}

	if (tone == 8) {
		for (unsigned i = 0; i < samples; i++) {
			toneVals.push_back(amplitude * (sin(x1*twoPi) + sin(x2*twoPi)));
			x1 += increment1_3;
			x2 += increment2_1;
		}
		return toneVals;
	}

	if (tone == 9) {
		for (unsigned i = 0; i < samples; i++) {
			toneVals.push_back(amplitude * (sin(x1*twoPi) + sin(x2*twoPi)));
			x1 += increment1_3;
			x2 += increment2_2;
		}
		return toneVals;
	}

	if (tone == 10) {
		for (int i = 0; i < samples; i++) {
			toneVals.push_back(amplitude * (sin(x1*twoPi) + sin(x2*twoPi)));
			x1 += increment1_3;
			x2 += increment2_3;
			/*std::cout << i << std::endl;
			std::cout << amplitude * (sin(x1*twoPi) + sin(x2*twoPi)) << std::endl;
			std::cout << toneVals[i] << std::endl;*/
		}
		return toneVals;
	}

	if (tone == 11) {
		for (unsigned i = 0; i < samples; i++) {
			toneVals.push_back(amplitude * (sin(x1*twoPi) + sin(x2*twoPi)));
			x1 += increment1_3;
			x2 += increment2_4;
		}
		return toneVals;
	}

	if (tone == 12) {
		for (unsigned i = 0; i < samples; i++) {
			toneVals.push_back(amplitude * (sin(x1*twoPi) + sin(x2*twoPi)));
			x1 += increment1_4;
			x2 += increment2_1;
		}
		return toneVals;
	}

	if (tone == 13) {
		for (unsigned i = 0; i < samples; i++) {
			toneVals.push_back(amplitude * (sin(x1*twoPi) + sin(x2*twoPi)));
			x1 += increment1_4;
			x2 += increment2_2;
		}
		return toneVals;
	}

	if (tone == 14) {
		for (unsigned i = 0; i < samples; i++) {
			toneVals.push_back(amplitude * (sin(x1*twoPi) + sin(x2*twoPi)));
			x1 += increment1_4;
			x2 += increment2_3;
		}
		return toneVals;
	}

	if (tone == 15) {
		for (unsigned i = 0; i < samples; i++) {
			toneVals.push_back(amplitude * (sin(x1*twoPi) + sin(x2*twoPi)));
			x1 += increment1_4;
			x2 += increment2_4;
		}
		return toneVals;
	}
	return toneVals;
}


DTMFToner::~DTMFToner()
{
}
