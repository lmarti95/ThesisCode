#include "OneMax.h"

OneMax::OneMax(int aN) : CostFunction(aN)
{
}

double OneMax::GetMaximumFitnessValue()
{
	return mN;
}

double OneMax::GetFitnessValue(int* aBitString)
{
	double sum = 0;

	for(int i = 0; i < mN; ++i)
	{
		sum += aBitString[i];
	}

	return sum;
}
