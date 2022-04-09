#include "Cliff.h"

Cliff::Cliff(int aN) : CostFunction(aN)
{
}

Cliff::Cliff(const Cliff& aOld) : CostFunction(aOld)
{
	mSum = 0;
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

	mSum = sum;

	if(sum < 2 * mN / 3)
	{
		return  sum;
	}

	return sum - mN / 3 + 1 / 2;
}

double Cliff::GetFitnessValue(int aChange)
{
	double sum = mSum + aChange;

	if(sum < 2 * mN / 3)
	{
		return  sum;
	}

	return sum - mN / 3 + 1 / 2;
}

void Cliff::CalculateSum(int* aBitString)
{
	double sum = 0;

	for(int i = 0; i < mN; ++i)
	{
		sum += aBitString[i];
	}

	mSum = sum;
}
