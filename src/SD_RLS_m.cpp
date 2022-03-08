#include "SD_RLS_m.h"

#include "Cliff.h"
#include "Jump.h"
#include "Utilityh.h"

#include <chrono>
#include <climits>
#include <cmath>
#include <float.h>

SD_RLS_m::SD_RLS_m(int aN, CostFunction* aCostFunction) : SD(aN, aCostFunction)
{
}

SD_RLS_m::~SD_RLS_m()
{
}

std::pair<long long, double> SD_RLS_m::RunEA()
{
	RandomizeBitString();
	double fitnessValue = mCostFunction->GetFitnessValue(mBitString);
	double newFitnessValue = 0;

	long long iterations = 0;
	bool justUpdated = false;

	int* bitStringPrime = new int[mN];

	double maximumFitnessValue = mCostFunction->GetMaximumFitnessValue();

	int u = 0;
	int s = 1;
	int r = 1;
	double B = DBL_MAX;

	auto start = std::chrono::steady_clock::now();

	while(fitnessValue != maximumFitnessValue)
	{
		iterations++;
		u++;

		if(!justUpdated)
		{
			std::copy(mBitString, mBitString + mN, bitStringPrime);
		}
		else
		{
			justUpdated = false;
		}

		int* selection = SelectRandomK(mN, s);

		int change = 0;

		for(int i = 0; i < s; ++i)
		{
			change += FlipBitBasedOnPosition(bitStringPrime, selection[i]);
		}

		delete[] selection;

		if(dynamic_cast<Jump*>(mCostFunction) != nullptr || dynamic_cast<Cliff*>(mCostFunction) != nullptr)
		{
			newFitnessValue = mCostFunction->GetFitnessValue(change);
		}
		else
		{
			newFitnessValue = mCostFunction->GetFitnessValue(bitStringPrime);
		}

		if(newFitnessValue > fitnessValue)
		{
			mCostFunction->ApplyChange(change);
			fitnessValue = newFitnessValue;
			std::copy(bitStringPrime, bitStringPrime + mN, mBitString);
			justUpdated = true;
			r = s;
			s = 1;
			if(r > 1)
			{
				B = (double)u / (double)(std::log(mN) * (r - 1));
			}
			else
			{
				B = DBL_MAX;
			}

			u = 0;
		}
		else
		{
			if(newFitnessValue == fitnessValue && r == 1)
			{
				std::copy(bitStringPrime, bitStringPrime + mN, mBitString);
				justUpdated = true;
			}
		}

		if(u > std::min(B,NoverK(mN, s) * std::log(mR)))
		{
			if(s == r)
			{
				if(r < mN / 2)
				{
					r++;
				}
				else
				{
					r = mN;
				}

				s = 1;
			}
			else
			{
				s++;
				if(s == r)
				{
					B = DBL_MAX;
				}
			}

			u = 0;
		}
	}

	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsedSeconds = end - start;

	delete[] bitStringPrime;

	return std::make_pair(iterations, elapsedSeconds.count());
}
