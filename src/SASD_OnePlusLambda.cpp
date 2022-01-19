#include "SASD_OnePlusLambda.h"

#include "include/OneMax.h"
#include "Utilityh.h"

#include <algorithm>
#include <chrono>
#include <cmath>

SASD_OnePlusLambda::SASD_OnePlusLambda(int aN, CostFunction* aCostFunction, int aLambda) : EvolutionaryAlgorithm(aN, aCostFunction)
{
	mBitString = new int[mN];
	mLambda = aLambda;
}

SASD_OnePlusLambda::~SASD_OnePlusLambda()
{
	delete[] mBitString;
}

void SASD_OnePlusLambda::RandomizeBitString()
{
	for(int i = 0; i < mN; ++i)
	{
		mBitString[i] = mCoin(mRng);
	}
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

	int fitnessValue = 0;

	if(dynamic_cast<OneMax*>(mCostFunction) != nullptr)
	{
		fitnessValue = mCostFunction->GetFitnessValue(mBitString);
	}

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
		int newFitnessValue = fitnessValue;

		for(int i = 0; i < mN; ++i)
		{
			if(CalculateFlipR())
			{
				if(bitStringPrime[i] == 0)
				{
					bitStringPrime[i] = 1;
					newFitnessValue++;
				}
				else
				{
					bitStringPrime[i] = 0;
					newFitnessValue--;
				}
			}
		}

		if(dynamic_cast<OneMax*>(mCostFunction) != nullptr)
		{
			offsprings->push_back(std::make_pair(bitStringPrime, newFitnessValue));
		}
		else
		{
			offsprings->push_back(std::make_pair(bitStringPrime, mCostFunction->GetFitnessValue(bitStringPrime)));
		}
	}

	return offsprings;
}

std::pair<long long, double> SASD_OnePlusLambda::RunEA()
{
	mR = 1;
	bool stagnationDetection = false;
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

			auto best = SelectBestDeleteRest(CreateOffsprings());
			if(best.second >= fitnessValue)
			{
				if(best.second > fitnessValue)
				{
					u = 0;
				}

				delete[] mBitString;
				mBitString = best.first;
				fitnessValue = best.second;
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

	return std::make_pair(iterations, elapsedSeconds.count());
}