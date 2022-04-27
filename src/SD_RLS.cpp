#include "SD_RLS.h"

#include "Cliff.h"
#include "Jump.h"
#include "Utilityh.h"

#include <chrono>
#include <cmath>
#include <thread>

SD_RLS::SD_RLS(int aN, CostFunction* aCostFunction) : SD(aN, aCostFunction)
{
}

SD_RLS::~SD_RLS()
{
}

std::pair<long long, double> SD_RLS::RunEA()
{
	RandomizeBitString();
	mCostFunction->CalculateSum(mBitString);
	mFitnessValue = mCostFunction->GetFitnessValue(0);
	double newFitnessValue = 0;

	bool justUpdated = false;
	mIterations = 0;

	int* bitStringPrime = new int[mN];

	double maximumFitnessValue = mCostFunction->GetMaximumFitnessValue();

	int u = 0;
	int s = 1;

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
		}
		else
		{
			justUpdated = false;
		}

		int* selection = SelectRandomK(mN, s);

		int change = 0;

		for(int i = 0; i < s; ++i)
		{
			change+=FlipBitBasedOnPosition(bitStringPrime, selection[i]);
		}

		delete[] selection;

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
			s = 1;
			u = 0;
		}
		else
		{
			if(newFitnessValue == mFitnessValue && s == 1)
			{
				#if GRAPHICS
					std::lock_guard<std::mutex> lg{mBitStringMutex};
				#endif

				std::copy(bitStringPrime, bitStringPrime + mN, mBitString);
				justUpdated = true;
			}
		}

		if(u > NoverK(mN, s) * std::log(mR))
		{
			mStagnationDetection = StagnationDetection::On;
			s++;
			if(s > mN)
			{
				s = mN;
			}
			u = 0;
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
