#include "Behandling.h"



Behandling::Behandling()
{
}

int Behandling::goertzler(int fs, int f, std::vector<int> *samples, int pStart, int sampleSize)
{
	float pi = 3.141592;
	float w1 = 0., w2 = 0., w0 = 0.;

	float n = (float)sampleSize;
	float k = floor((n*f) / fs);
	float omega = (float)(2 * (pi / n))*k;
	float cosine = (float)cos(omega);
	float coeff = 2 * cosine;
	

	for (int i = pStart; i < pStart+sampleSize; i++)
	{
		w0 = coeff * w1 - w2 + (*samples)[i];
		w2 = w1;
		w1 = w0;
	}
	int magnitude = sqrt(w1*w1 + w2 * w2 - w1 * w2*coeff);
	return magnitude;
}

int Behandling::bestLow(int fs, std::vector<int> *samples, int pStart, int sampleSize)
{
	int a = goertzler(fs, 697, samples, pStart, sampleSize);
	int b = goertzler(fs, 770, samples, pStart, sampleSize);
	int c = goertzler(fs, 852, samples, pStart, sampleSize);
	int d = goertzler(fs, 941, samples, pStart, sampleSize);
	int test[4] = { a,b,c,d };
	int max = 0;
	for (int i = 0; i < 4; i++) {
		if (max < test[i])
			max = test[i];
	}
	if (max = a)
		return 697;
	else if (max = b)
		return 770;
	else if (max = c)
		return 852;
	else if (max = d)
		return 941;
	else
		return 0;
}

int Behandling::bestHigh(int fs, std::vector<int> *samples, int pStart, int sampleSize)
{
	int a = goertzler(fs, 1209, samples, pStart, sampleSize);
	int b = goertzler(fs, 1336, samples, pStart, sampleSize);
	int c = goertzler(fs, 1477, samples, pStart, sampleSize);
	int d = goertzler(fs, 1633, samples, pStart, sampleSize);
	int test[4] = { a,b,c,d };
	int max = 0;
	for (int i = 0; i < 4; i++) {
		if (max > test[i])
			max = test[i];
	}
	if (max = a)
		return 1209;
	else if (max = b)
		return 1336;
	else if (max = c)
		return 1477;
	else if (max = d)
		return 1633;
	else
		return 0;
}



Behandling::~Behandling()
{
}
