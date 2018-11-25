#include "Behandling.h"



Behandling::Behandling()
{
}

int Behandling::goertzler(int fs, int f, std::vector<int> samples)
{
	float n = (float)samples.size();
	float k = floor((n*f) / fs);
	float omega = ((2.*3.14159265) / n)*k;
	float cosine = cos(omega);
	float coeff = 2 * cosine;

	float w1 = 0., w2 = 0., w0 = 0.;
	for (int i = 0; i < n - 1; i++)
	{
		w0 = coeff * w1 - w2 + samples[i];
		w2 = w1;
		w1 = w0;
	}
	int magnetude = sqrt(w1*w1 + w2 * w2 - w1 * w2*coeff);
	return magnetude;
}

int Behandling::bestLow(int fs, std::vector<int> samples)
{
	int a = goertzler(fs, 697, samples);
	int b = goertzler(fs, 770, samples);
	int c = goertzler(fs, 852, samples);
	int d = goertzler(fs, 941, samples);
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

int Behandling::bestHigh(int fs, std::vector<int> samples)
{
	int a = goertzler(fs, 1209, samples);
	int b = goertzler(fs, 1336, samples);
	int c = goertzler(fs, 1477, samples);
	int d = goertzler(fs, 1633, samples);
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
