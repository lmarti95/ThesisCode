#include "SD_OnePlusOne.h"

#include "Cliff.h"
#include "Jump.h"
#include "Utilityh.h"

#include <algorithm>
#include <chrono>
#include <cmath>

SD_OnePlusOne::SD_OnePlusOne(int aN, CostFunction* aCostFunction) : SD(aN, aCostFunction)
{
}

SD_OnePlusOne::~SD_OnePlusOne()
{
}

bool SD_OnePlusOne::CalculateFlipR()
{
	std::uniform_real_distribution<> p(0.0, 1.0);

	if(p(mRng) > (double)mR / (double)mN)
	{
		return true;
	}

	return false;
}


std::pair<long long, double> SD_OnePlusOne::RunEA()
{
	mR = 1;
	RandomizeBitString();
	double fitnessValue = mCostFunction->GetFitnessValue(mBitString);
	double newFitnessValue = 0;

	long long iterations = 0;
	bool justUpdated = false;

	int* bitStringPrime = new int[mN];

	double maximumFitnessValue = mCostFunction->GetMaximumFitnessValue();

	int u = 0;

	auto start = std::chrono::steady_clock::now();

	while(fitnessValue != maximumFitnessValue)
	{
		iterations++;
		u++;

		if(!justUpdated)
		{
			std::copy(mBitString, mBitString + mN, bitStringPrime);
			newFitnessValue = fitnessValue;
		}
		else
		{
			justUpdated = false;
		}

		int change = 0;

		for(int i = 0; i < mN; ++i)
		{
			if(CalculateFlipR())
			{
				change+=FlipBitBasedOnPosition(bitStringPrime, i);
			}
		}

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
			mR = 1;
			u = 0;
		}
		else
		{
			if(newFitnessValue == fitnessValue && mR == 1)
			{
				std::copy(bitStringPrime, bitStringPrime + mN, mBitString);
				justUpdated = true;
			}
		}

		if(u > 2* std::pow((std::exp(1.0)*mN/mR),mR)* std::log(mN*mR))
		{
			mR = std::min(mR + 1, mN / 2);
			u = 0;
		}
		else
		{
			mR++;
		}
	}

	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsedSeconds = end - start;

	delete[] bitStringPrime;

	return std::make_pair(iterations, elapsedSeconds.count());
}