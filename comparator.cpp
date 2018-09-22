#include "comparator.h"

// given two histograms of size nBins returns 
// the probability that they are the same
double intersection(double* hist1, double* hist2, int nBins)
{
	double sum = 0;
	for (int i = 0; i < nBins; i++)
	{
		// sum += min(hist1[i], hist2[i])
		sum += (hist1[i] < hist2[i]) ? hist1[i] : hist2[i];
	}
	return sum;
}