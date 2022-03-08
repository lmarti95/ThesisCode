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

bool SASD_OnePlusLambda::CalculateFlipR()
{
	std::uniform_real_distribution<> p(0.0, 1.0);

	if(p(mRng) > (double)(mNumeratorMultiplier*mR)/ (double)(mDenominatorMultiplier*mN))
	{
		return true;
	}

	return false;
}


std::vector<std::pair<int*, double>>* SASD_OnePlusLambda::CreateOffsprings()
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
			if(CalculateFlipR())
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
	mR = 1;
	bool stagnationDetection = false;
	RandomizeBitString();
	double fitnessValue = mCostFunction->GetFitnessValue(mBitString);

	long long iterations = 0;

	int* bitStringPrime = new int[mN];

	double maximumFitnessValue = mCostFunction->GetMaximumFitnessValue();

	int u = 0;

	auto start = std::chrono::steady_clock::now();

	while(fitnessValue != maximumFitnessValue)
	{
		iterations++;
		u++;

		if(stagnationDetection)
		{
			mDenominatorMultiplier = 1;
			mNumeratorMultiplier = 1;

			auto best = SelectBestDeleteRest(CreateOffsprings());

			if(best.second > fitnessValue)
			{
				delete[] mBitString;
				mBitString = best.first;
				fitnessValue = best.second;
				mR = 1;
				stagnationDetection = false;
				u = 0;
			}
			else
			{
				if(u > 2 * std::pow((std::exp(1.0) * (double)mN / (double)mR), mR) * std::log(mN * mR) / (double)mLambda)
				{
					mR = std::min(mR + 1, mN / 2);
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
			auto best = SelectBestDeleteRest(CreateOffsprings(), &RRate);
			if(best.second >= fitnessValue)
			{
				if(best.second > fitnessValue)
				{
					u = 0;
				}

				delete[] mBitString;
				mBitString = best.first;
				fitnessValue = best.second;
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
					mR = mR / 2;
				}
				else
				{
					mR = 2 * mR;
				}
			}
			else
			{
				if(mLastRRate == 0)
				{
					mR = mR / 2;
				}
				else
				{
					mR = mR * 2;
				}
			}

			mR = std::min(std::max(2, mR), mN / 4);

			if(u > 2 * std::pow((std::exp(1.0) * (double)mN / (double)mR), mR) * std::log(mN * mR) / (double)mLambda)
			{
				mR = 2;
				u = 0;
				stagnationDetection = true;
			}
		}
	}

	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsedSeconds = end - start;

	delete[] bitStringPrime;

	return std::make_pair(iterations*mLambda, elapsedSeconds.count());
}
