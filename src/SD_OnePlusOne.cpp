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

bool SD_OnePlusOne::CalculateFlipR(double aR)
{
	std::uniform_real_distribution<> p(0.0, 1.0);

	if(p(mRng) <= aR / (double)mN)
	{
		return true;
	}

	return false;
}


std::pair<long long, double> SD_OnePlusOne::RunEA()
{
	double r = 1;
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
			#if GRAPHICS
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
			if(CalculateFlipR(r))
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
			#if GRAPHICS
				std::lock_guard<std::mutex> lg{mBitStringMutex};
			#endif

			mCostFunction->ApplyChange(change);
			mFitnessValue = newFitnessValue;
			std::copy(bitStringPrime, bitStringPrime + mN, mBitString);
			justUpdated = true;
			r = 1;
			u = 0;
		}
		else
		{
			if(newFitnessValue == mFitnessValue && r == 1)
			{
				#if GRAPHICS
					std::lock_guard<std::mutex> lg{mBitStringMutex};
				#endif

				std::copy(bitStringPrime, bitStringPrime + mN, mBitString);
				justUpdated = true;
			}
		}
		double firstPart = std::pow((std::exp(1.0) * mN / r), r);
		double secondPart = std::log(mN * mR);
		double calculated = 2 * firstPart * secondPart;
		if(u > calculated)
		{
			r = std::min(r + 1, (double)mN / 2);
			u = 0;
			mStagnationDetection = StagnationDetection::On;
		}

		#if GRAPHICS
			std::this_thread::sleep_for(std::chrono::milliseconds(mDelay));
		#endif
	}

	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsedSeconds = end - start;

	delete[] bitStringPrime;

	long long iterations = mIterations;

	return std::make_pair(iterations, elapsedSeconds.count());
}