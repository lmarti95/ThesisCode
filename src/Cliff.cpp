#include "Cliff.h"

Cliff::Cliff(int aN) : CostFunction(aN)
{
}

double Cliff::GetMaximumFitnessValue()
{
	return mN - mN / 3 + 1 / 2;
}

double Cliff::GetFitnessValue(int* aBitString)
{
	double sum = 0;

	for(int i = 0; i < mN; ++i)
	{
		sum += aBitString[i];
	}

	if(sum < 2 * mN / 3)
	{
		return  sum;
	}

	return sum - mN / 3 + 1 / 2;
}