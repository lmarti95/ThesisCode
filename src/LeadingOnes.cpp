#include "LeadingOnes.h"

LeadingOnes::LeadingOnes(int aN) : CostFunction(aN)
{
}

double LeadingOnes::GetMaximumFitnessValue()
{
	return mN;
}

double LeadingOnes::GetFitnessValue(int* aBitString)
{
	double max1s = 0;
	double temp = 0;

	for(int i = 0; i < mN; ++i)
	{
		if(aBitString[i] == 1)
		{
			temp++;
		}
		else
		{
			if(temp > max1s)
			{
				max1s = temp;
			}
			temp = 0;
		}
	}

	if(temp > max1s)
	{
		max1s = temp;
	}

	return max1s;
}
