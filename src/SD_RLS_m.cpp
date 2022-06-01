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
	delete mCostFunction;
	delete[] mBitString;
}

std::pair<long long, double> SD_RLS_m::RunEA()
{
	RandomizeBitString();
	mCostFunction->CalculateSum(mBitString);
	mFitnessValue = mCostFunction->GetFitnessValue(0);
	double newFitnessValue = 0;

	mIterations = 0;
	bool justUpdated = false;

	int* bitStringPrime = new int[mN];

	double maximumFitnessValue = mCostFunction->GetMaximumFitnessValue();

	int u = 0;
	int s = 1;
	int r = 1;
	double B = DBL_MAX;

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
			change += FlipBitBasedOnPosition(bitStringPrime, selection[i]);
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
			mCostFunction->ApplyChange(change);
			mFitnessValue = newFitnessValue;

			#if GRAPHICS
				std::lock_guard<std::mutex> lg{mBitStringMutex};
			#endif

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
			if(newFitnessValue == mFitnessValue && r == 1)
			{
				#if GRAPHICS
					std::lock_guard<std::mutex> lg{mBitStringMutex};
				#endif

				std::copy(bitStringPrime, bitStringPrime + mN, mBitString);
				justUpdated = true;
			}
		}

		if(u > std::min(B,NoverK(mN, s) * std::log(mR)))
		{
			mStagnationDetection = StagnationDetection::On;
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
