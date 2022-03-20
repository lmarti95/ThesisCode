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
	mCostFunction->CalculateSum(mBitString);
	mFitnessValue = mCostFunction->GetFitnessValue(0);
	double newFitnessValue = 0;

	mIterations = 0;
	bool justUpdated = false;

	int* bitStringPrime = new int[mN];

	double maximumFitnessValue = mCostFunction->GetMaximumFitnessValue();

	int u = 0;

	auto start = std::chrono::steady_clock::now();

	while(mFitnessValue != maximumFitnessValue)
	{
		mIterations++;
		u++;

		if(!justUpdated)
		{
			#ifdef GRAPHICS
				std::lock_guard<std::mutex> lg{mBitStringMutex};
			#endif

			std::copy(mBitString, mBitString + mN, bitStringPrime);
			newFitnessValue = mFitnessValue;
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

		if(mFitnessChangePossible)
		{
			newFitnessValue = mCostFunction->GetFitnessValue(change);
		}
		else
		{
			newFitnessValue = mCostFunction->GetFitnessValue(bitStringPrime);
		}

		if(newFitnessValue > mFitnessValue)
		{
			#ifdef GRAPHICS
				std::lock_guard<std::mutex> lg{mBitStringMutex};
			#endif

			mCostFunction->ApplyChange(change);
			mFitnessValue = newFitnessValue;
			std::copy(bitStringPrime, bitStringPrime + mN, mBitString);
			justUpdated = true;
			mR = 1;
			u = 0;
		}
		else
		{
			if(newFitnessValue == mFitnessValue && mR == 1)
			{
				#ifdef GRAPHICS
					std::lock_guard<std::mutex> lg{mBitStringMutex};
				#endif

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

		#ifdef GRAPHICS
			std::this_thread::sleep_for(std::chrono::milliseconds(mDelay));
		#endif
	}

	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsedSeconds = end - start;

	delete[] bitStringPrime;

	long long iterations = mIterations;

	return std::make_pair(iterations, elapsedSeconds.count());
}