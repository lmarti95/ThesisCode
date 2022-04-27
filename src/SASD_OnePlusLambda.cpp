#include "SASD_OnePlusLambda.h"

#include "Utilityh.h"

#include <algorithm>
#include <chrono>
#include <cmath>

SASD_OnePlusLambda::SASD_OnePlusLambda(int aN, CostFunction* aCostFunction, int aLambda) : SD(aN, aCostFunction)
{
	mLambda = aLambda;
}

SASD_OnePlusLambda::~SASD_OnePlusLambda()
{
}

bool SASD_OnePlusLambda::CalculateFlipR(double aR)
{
	std::uniform_real_distribution<> p(0.0, 1.0);

	if(p(mRng) <= (double)(mNumeratorMultiplier*aR)/ (double)(mDenominatorMultiplier*mN))
	{
		return true;
	}

	return false;
}


std::vector<std::pair<int*, double>>* SASD_OnePlusLambda::CreateOffsprings(double aR)
{
	std::vector<std::pair<int*, double>>* offsprings = new std::vector<std::pair<int*, double>>;

	for(int i = 0; i < mLambda; ++i)
	{
		if(mNumeratorMultiplier != 1 || mDenominatorMultiplier != 1)
		{
			if(i  <= mLambda / 2)
			{
				mNumeratorMultiplier = 1;
				mDenominatorMultiplier = 2;
			}
			else
			{
				mNumeratorMultiplier = 2;
				mDenominatorMultiplier = 1;
			}
		}

		int* bitStringPrime = new int[mN];

		std::copy(mBitString, mBitString + mN, bitStringPrime);

		for(int i = 0; i < mN; ++i)
		{
			if(CalculateFlipR(aR))
			{
				FlipBitBasedOnPosition(bitStringPrime, i);
			}
		}

		offsprings->push_back(std::make_pair(bitStringPrime, mCostFunction->GetFitnessValue(bitStringPrime)));
	}

	return offsprings;
}

std::pair<long long, double> SASD_OnePlusLambda::RunEA()
{
	double r = 1;
	bool stagnationDetection = false;
	RandomizeBitString();
	mCostFunction->CalculateSum(mBitString);
	mFitnessValue = mCostFunction->GetFitnessValue(0);

	mIterations = 0;

	int* bitStringPrime = new int[mN];

	double maximumFitnessValue = mCostFunction->GetMaximumFitnessValue();

	int u = 0;

	auto start = std::chrono::steady_clock::now();

	while(mFitnessValue != maximumFitnessValue)
	{
		mIterations++;
		u++;

		if(stagnationDetection)
		{
			mDenominatorMultiplier = 1;
			mNumeratorMultiplier = 1;

			auto best = SelectBestDeleteRest(CreateOffsprings(r));

			if(best.second > mFitnessValue)
			{
				#if GRAPHICS
					std::lock_guard<std::mutex> lg{mBitStringMutex};
				#endif

				delete[] mBitString;
				mBitString = best.first;
				mFitnessValue = best.second;

				#if GRAPHICS
					mCostFunction->CalculateSum(mBitString);
				#endif
				
				r = 1;
				stagnationDetection = false;
				u = 0;
			}
			else
			{
				if(u > 2 * std::pow((std::exp(1.0) * (double)mN / (double)r), r) * std::log(mN * mR) / (double)mLambda)
				{
					r = std::min(r + 1, (double)mN / 2);
					u = 0;
				}

				delete[] best.first;
			}
		}
		else
		{
			mDenominatorMultiplier = 2;
			mNumeratorMultiplier = 1;

			int RRate = -1;
			auto best = SelectBestDeleteRest(CreateOffsprings(r), &RRate);
			if(best.second >= mFitnessValue)
			{
				if(best.second > mFitnessValue)
				{
					u = 0;
				}
				#if GRAPHICS
					std::lock_guard<std::mutex> lg{mBitStringMutex};
				#endif

				delete[] mBitString;
				mBitString = best.first;

				#if GRAPHICS
					mCostFunction->CalculateSum(mBitString);
				#endif

				mFitnessValue = best.second;
				mLastRRate = RRate;
			}
			else
			{
				delete[] best.first;
			}

			if(mCoin(mRng))
			{
				if(mCoin(mRng))
				{
					r = r / 2;
				}
				else
				{
					r = 2 * r;
				}
			}
			else
			{
				if(mLastRRate == 0)
				{
					r = r / 2;
				}
				else
				{
					r = r * 2;
				}
			}

			r = std::min(std::max((double)2, r), (double)mN / 4);

			if(u > 2 * std::pow((std::exp(1.0) * (double)mN / (double)r), r) * std::log(mN * mR) / (double)mLambda)
			{
				r = 2;
				u = 0;
				stagnationDetection = true;
				mStagnationDetection = StagnationDetection::On;
			}
		}

		#if GRAPHICS
			std::this_thread::sleep_for(std::chrono::milliseconds(mDelay));
		#endif
	}

	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsedSeconds = end - start;

	delete[] bitStringPrime;

	long long iterations = mIterations;

	return std::make_pair(iterations*mLambda, elapsedSeconds.count());
}
