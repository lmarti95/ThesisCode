#include "OnePlusOneEA.h"

#include"OneMax.h"

#include <chrono>

OnePlusOneEA::OnePlusOneEA(int aN, CostFunction* aCostFunction) : EvolutionaryAlgorithm(aN, aCostFunction)
{
	mBitString = new int[mN];

	mOptimizationLevel = OnePlusOneEAOptimization::All;
}

OnePlusOneEA::~OnePlusOneEA()
{
	delete[] mBitString;
}

void OnePlusOneEA::RandomizeBitString()
{
	for(int i = 0; i < mN; ++i)
	{
		mBitString[i] = mCoin(mRng);
	}
}

std::pair<long long, double> OnePlusOneEA::RunEA()
{
	RandomizeBitString();
	mNextPosition = -1;

	switch(mOptimizationLevel)
	{
		case OnePlusOneEAOptimization::No:
			return NoOptimizationOneOne();
		case OnePlusOneEAOptimization::FitnessEvaluation:
			return FitnessEvaluationOptimizationOneOne();
		case OnePlusOneEAOptimization::RandomNumberGeneration:
			return RandomNumberGenerationOptimizationOneOne();
		case OnePlusOneEAOptimization::All:
			return AllOptimizationOneOne();
		default:
			return AllOptimizationOneOne();
	}
}

std::pair<long long, double> OnePlusOneEA::NoOptimizationOneOne()
{
	double fitnessValue = mCostFunction->GetFitnessValue(mBitString);
	double newFitnessValue = 0;

	long long iterations = 0;
	bool justUpdated = false;

	int* bitStringPrime = new int[mN];

	double maximumFitnessValue = mCostFunction->GetMaximumFitnessValue();

	auto start = std::chrono::steady_clock::now();

	while(fitnessValue != maximumFitnessValue)
	{
		iterations++;

		if(!justUpdated)
		{
			std::copy(mBitString, mBitString + mN, bitStringPrime);
		}
		else
		{
			justUpdated = false;
		}

		for(int i = 0; i < mN; ++i)
		{
			if(mRandomN(mRng) == mN)
			{
				if(bitStringPrime[i] == 0)
				{
					bitStringPrime[i] = 1;
				}
				else
				{
					bitStringPrime[i] = 0;
				}
			}
		}

		newFitnessValue = mCostFunction->GetFitnessValue(bitStringPrime);

		if(newFitnessValue > fitnessValue)
		{
			fitnessValue = newFitnessValue;
			std::copy(bitStringPrime, bitStringPrime + mN, mBitString);
			justUpdated = true;
		}
	}

	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsedSeconds = end - start;

	delete[] bitStringPrime;

	return std::make_pair(iterations, elapsedSeconds.count());
}

std::pair<long long, double> OnePlusOneEA::FitnessEvaluationOptimizationOneOne()
{
	double fitnessValue = mCostFunction->GetFitnessValue(mBitString);
	double newFitnessValue = 0;

	long long iterations = 0;
	bool justUpdated = false;

	int* bitStringPrime = new int[mN];

	double maximumFitnessValue = mCostFunction->GetMaximumFitnessValue();

	auto start = std::chrono::steady_clock::now();

	while(fitnessValue != maximumFitnessValue)
	{
		iterations++;

		if(!justUpdated)
		{
			std::copy(mBitString, mBitString + mN, bitStringPrime);
			newFitnessValue = fitnessValue;
		}
		else
		{
			justUpdated = false;
		}

		for(int i = 0; i < mN; ++i)
		{
			if(mRandomN(mRng) == mN)
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

		if(dynamic_cast<OneMax*>(mCostFunction) == nullptr)
		{
			newFitnessValue = mCostFunction->GetFitnessValue(bitStringPrime);
		}

		if(newFitnessValue > fitnessValue)
		{
			fitnessValue = newFitnessValue;
			std::copy(bitStringPrime, bitStringPrime + mN, mBitString);
			justUpdated = true;
		}
	}

	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsedSeconds = end - start;

	delete[] bitStringPrime;

	return std::make_pair(iterations, elapsedSeconds.count());
}

std::pair<long long, double> OnePlusOneEA::RandomNumberGenerationOptimizationOneOne()
{
	double fitnessValue = mCostFunction->GetFitnessValue(mBitString);
	double newFitnessValue = 0;

	long long iterations = 0;
	bool justUpdated = false;

	int* bitStringPrime = new int[mN];

	double maximumFitnessValue = mCostFunction->GetMaximumFitnessValue();

	auto start = std::chrono::steady_clock::now();

	while(fitnessValue != maximumFitnessValue)
	{
		iterations++;

		if(!justUpdated)
		{
			std::copy(mBitString, mBitString + mN, bitStringPrime);
		}
		else
		{
			justUpdated = false;
		}

		long next = GetNextPosition(mN - 1);
		if(next != -1)
		{
			FlipBits(bitStringPrime, next);
		}
		else
		{
			continue;
		}

		newFitnessValue = mCostFunction->GetFitnessValue(bitStringPrime);

		if(newFitnessValue > fitnessValue)
		{
			fitnessValue = newFitnessValue;
			std::copy(bitStringPrime, bitStringPrime + mN, mBitString);
			justUpdated = true;
		}
	}

	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsedSeconds = end - start;

	delete[] bitStringPrime;

	return std::make_pair(iterations, elapsedSeconds.count());
}

std::pair<long long, double> OnePlusOneEA::AllOptimizationOneOne()
{
	double fitnessValue = mCostFunction->GetFitnessValue(mBitString);
	double newFitnessValue = 0;

	long long iterations = 0;
	bool justUpdated = false;

	int* bitStringPrime = new int[mN];

	double maximumFitnessValue = mCostFunction->GetMaximumFitnessValue();

	auto start = std::chrono::steady_clock::now();

	while(fitnessValue != maximumFitnessValue)
	{
		iterations++;

		if(!justUpdated)
		{
			std::copy(mBitString, mBitString + mN, bitStringPrime);
		}
		else
		{
			justUpdated = false;
		}

		long next = GetNextPosition(mN - 1);
		if(next != -1)
		{
			newFitnessValue = FlipBits(bitStringPrime, next) + fitnessValue;
		}
		else
		{
			continue;
		}

		if(dynamic_cast<OneMax*>(mCostFunction) == nullptr)
		{
			newFitnessValue = mCostFunction->GetFitnessValue(bitStringPrime);
		}

		if(newFitnessValue > fitnessValue)
		{
			fitnessValue = newFitnessValue;
			std::copy(bitStringPrime, bitStringPrime + mN, mBitString);
			justUpdated = true;
		}
	}

	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsedSeconds = end - start;

	delete[] bitStringPrime;

	return std::make_pair(iterations, elapsedSeconds.count());
}