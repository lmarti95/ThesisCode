#include "SD_RLS.h"

#include "Utilityh.h"

#include <chrono>
#include <cmath>

#include <iostream>

SD_RLS::SD_RLS(int aN, CostFunction* aCostFunction) : EvolutionaryAlgorithm(aN, aCostFunction)
{
	mBitString = new int[mN];
	mR = std::pow(mN, 3 + mEpsilon);
}

SD_RLS::~SD_RLS()
{
	delete[] mBitString;
}

void SD_RLS::SetEpsilon(int aEpsilon)
{
	mEpsilon = aEpsilon;
}

void SD_RLS::RandomizeBitString()
{
	for(int i = 0; i < mN; ++i)
	{
		mBitString[i] = mCoin(mRng);
	}
}


std::pair<long long, double> SD_RLS::RunEA()
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
			if(bitStringPrime[selection[i]] == 1)
			{
				bitStringPrime[selection[i]] = 0;
			}
			else
			{
				bitStringPrime[selection[i]] = 1;
			}
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
		}
		else
		{
			if(newFitnessValue == fitnessValue && s == 1)
			{
				std::copy(bitStringPrime, bitStringPrime + mN, mBitString);
				justUpdated = true;
			}
		}

		if(u > NoverK(mN, s) * std::log(mR))
		{
			s++;
			if(s > mN)
			{
				s = mN;
			}
			u = 0;
		}
	}

	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsedSeconds = end - start;

	delete[] bitStringPrime;

	return std::make_pair(iterations, elapsedSeconds.count());
}