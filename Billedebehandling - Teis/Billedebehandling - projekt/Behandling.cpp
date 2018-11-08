#include "Behandling.h"



Behandling::Behandling()
{
}

int Behandling::goertzler(int fs, int f, std::vector<int> sampels)
{
	int n = sampels.size();
	int magnitude = 0;
	int w0 = 0;
	int w1 = 0;
	int w2 = 0;
	
	int k = floor((0.5 + (n*f) / fs));
	float omega = (2 * 3.14159265 / n)*k;
	float cosine = cos(omega);
	float sine = sin(omega);
	float coeff = 2 * cosine;

	for (int i = 0; i < n - 1; i++) {
		w0 = coeff * w1 - w2 + sampels[i];
		w2 = w1;
		w1 = w0;
	}

	magnitude = sqrt(w1*w1 + w2 * w2 - w1 * w2*coeff);
	return magnitude;
}

int Behandling::bestLowDTMF(int fs, std::vector<int> sampels)
{
	int a = 1;
	int lowDTMF[4] = { 697, 770, 852, 941 };
	
	for (int i = 0; i < 4; i++) {											// Finder den frekvens med højest værdi blandt de lave DTMF toner
		if (goertzler(samplingFrekvens, lowDTMF[i], maalinger) > goertzler(samplingFrekvens, a, maalinger)) {
			a = lowDTMF[i];
		}
	}

	return a;
}


int Behandling::bestHighDTMF(int fs, std::vector<float> sampels)
{
	int a = 1;
	int highDTMF[4] = { 1209, 1336,1477,1633 };								
	for (int i = 0; i < 4; i++) {											// Finder den frekvens med den højeste værdi blandt de høje DTMF toner
		if (goertzler(samplingFrekvens, highDTMF[i], maalinger) > goertzler(samplingFrekvens, a, maalinger)) {			// måskke kan denne optimeres så gørtzler ikke bliver kaldt to gange
			a = highDTMF[i];																							// EVT. at den husker den forrige værdi

		}
	}
	return a;
}

Behandling::~Behandling()
{
}
