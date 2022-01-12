#include "Jump.h"

Jump::Jump(int aN, int aGapSize) : CostFunction(aN)
{
	mGapSize = aGapSize;
}

double Jump::GetMaximumFitnessValue()
{
	return mN + mGapSize;
}

double Jump::GetFitnessValue(int* aBitString)
{
	double sum = 0;

	for(int i = 0; i < mN; ++i)
	{
		sum += aBitString[i];
	}

	if(sum > mN - mGapSize && sum != mN)
	{
		return  mN - sum;
	}

	return sum + mGapSize;
}