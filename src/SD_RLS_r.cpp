#include "SD_RLS_r.h"

#include "Utilityh.h"

#include <chrono>
#include <cmath>

SD_RLS_r::SD_RLS_r(int aN, CostFunction* aCostFunction) : SD(aN, aCostFunction)
{
}

SD_RLS_r::~SD_RLS_r()
{
}

std::pair<long long, double> SD_RLS_r::RunEA()
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

		for(int i = 0; i < s; ++i)
		{
			FlipBitBasedOnPosition(bitStringPrime, selection[i]);
		}

		delete[] selection;

		newFitnessValue = mCostFunction->GetFitnessValue(bitStringPrime);

		if(newFitnessValue > fitnessValue)
		{
			fitnessValue = newFitnessValue;
			std::copy(bitStringPrime, bitStringPrime + mN, mBitString);
			justUpdated = true;
			s = 1;
			u = 0;
			r = 1;
		}
		else
		{
			if(newFitnessValue == fitnessValue && r == 1)
			{
				std::copy(bitStringPrime, bitStringPrime + mN, mBitString);
				justUpdated = true;
			}
		}

		if(u > NoverK(mN, s) * std::log(mR))
		{
			if(s == 1)
			{
				if(r < mN / 2)
				{
					r++;
				}
				else
				{
					r = mN;
				}

				s = r;
			}
			else
			{
				s--;
			}

			u = 0;
		}
	}

	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsedSeconds = end - start;

	delete[] bitStringPrime;

	return std::make_pair(iterations, elapsedSeconds.count());
}