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
	int a = 941;
	int lowDTMF[4] = { 697, 770, 852, 941 };
	
	for (int i = 0; i < 4; i++) {											// Finder den frekvens med højest værdi blandt de lave DTMF toner
		if (goertzler(fs, lowDTMF[i], sampels) > goertzler(fs, a, sampels)) {
			a = lowDTMF[i];
		}
	}

	return a;
}


int Behandling::bestHighDTMF(int fs, std::vector<int> sampels)
{
	int a = 1633;
	int highDTMF[4] = { 1209, 1336,1477,1633 };								
	for (int i = 0; i < 4; i++) {											// Finder den frekvens med den højeste værdi blandt de høje DTMF toner
		if (goertzler(fs, highDTMF[i], sampels) > goertzler(fs, a, sampels)) {			// måskke kan denne optimeres så gørtzler ikke bliver kaldt to gange
			a = highDTMF[i];																							// EVT. at den husker den forrige værdi

		}
	}
	return a;
}

Behandling::~Behandling()
{
}
