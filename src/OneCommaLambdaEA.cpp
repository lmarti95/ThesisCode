#include "OneCommaLambdaEA.h"

#include "OneMax.h"

#include <chrono>

OneCommaLambdaEA::OneCommaLambdaEA(int N, CostFunction* aCostFunction,int aLambda) : EvolutionaryAlgorithm(N, aCostFunction)
{
	mLambda = aLambda;
	mBitString = new int[mN];
}

OneCommaLambdaEA::~OneCommaLambdaEA()
{
	delete[] mBitString;
}

void OneCommaLambdaEA::RandomizeBitString()
{
	for(int i = 0; i < mN; ++i)
	{
		mBitString[i] = mCoin(mRng);
	}
}

std::vector<std::pair<int*, double>>* OneCommaLambdaEA::CreateOffsprings()
{
	std::vector<std::pair<int*, double>>* offsprings = new std::vector<std::pair<int*, double>>;

	int fitnessValue = 0;

	if(dynamic_cast<OneMax*>(mCostFunction) != nullptr)
	{
		fitnessValue = mCostFunction->GetFitnessValue(mBitString);
	}

	for(int i = 0; i < mLambda; ++i)
	{
		int* bitStringPrime = new int[mN];

		std::copy(mBitString, mBitString + mN, bitStringPrime);
		int newFitnessValue = fitnessValue;

		long next = GetNextPosition(mN - 1);
		if(next != -1)
		{
			newFitnessValue = FlipBits(bitStringPrime, next) + fitnessValue;
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

	delete[] mBitString;

	return offsprings;
}

std::pair<long long, double> OneCommaLambdaEA::RunEA()
{
	RandomizeBitString();

	mFitnessValue = mCostFunction->GetFitnessValue(mBitString);

	long long iterations = 0;

	double maximumFitnessValue = mCostFunction->GetMaximumFitnessValue();

	auto start = std::chrono::steady_clock::now();

	while(maximumFitnessValue != mFitnessValue)
	{
		iterations++;

		auto bitStringPrime = SelectBestDeleteRest(CreateOffsprings());

		mBitString = bitStringPrime.first;
		mFitnessValue = bitStringPrime.second;
	}

	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsedSeconds = end - start;

	return std::make_pair(iterations, elapsedSeconds.count());
}
