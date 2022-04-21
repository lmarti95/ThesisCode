#include "SD_RLS_STAR.h"

#include "Cliff.h"
#include "Jump.h"
#include "Utilityh.h"

#include <chrono>
#include <cmath>

SD_RLS_STAR::SD_RLS_STAR(int aN, CostFunction* aCostFunction) : SD(aN, aCostFunction)
{
}

SD_RLS_STAR::~SD_RLS_STAR()
{
}

std::pair<long long, double> SD_RLS_STAR::RunEA()
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

			#ifdef GRAPHICS
				std::lock_guard<std::mutex> lg{mBitStringMutex};
			#endif
			
			std::copy(bitStringPrime, bitStringPrime + mN, mBitString);
			justUpdated = true;
			s = 1;
			u = 0;
			r = 1;
		}
		else
		{
			if(newFitnessValue == mFitnessValue && r == 1)
			{

			#ifdef GRAPHICS
					std::lock_guard<std::mutex> lg{mBitStringMutex};
			#endif

				std::copy(bitStringPrime, bitStringPrime + mN, mBitString);
				justUpdated = true;
			}
		}

		if(u > NoverK(mN, s) * std::log(mR))
		{
			mStagnationDetection = StagnationDetection::On;
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
				s = s - 1;
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