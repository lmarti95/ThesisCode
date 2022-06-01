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
	return (double)(mN - mN / 3) + 0.5;
}

double Cliff::GetFitnessValue(int* aBitString)
{
	double sum = 0;

	for(int i = 0; i < mN; ++i)
	{
		sum += aBitString[i];
	}

	mSum = sum;

	if(sum <= mN / 3 * 2)
	{
		return  sum;
	}

	return (double)(sum - mN / 3) + 0.5;
}

double Cliff::GetFitnessValue(int aChange)
{
	double sum = mSum + aChange;

	if(sum <=  mN / 3*2)
	{
		return  sum;
	}

	return (double)(sum - mN / 3) + 0.5;
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

double Cliff::FitnessValueToSum(double aFitness)
{
	if((int)aFitness == aFitness)
	{
		return aFitness;
	}
	else
	{
		return aFitness - 0.5 + mN / 3;
	}
}
