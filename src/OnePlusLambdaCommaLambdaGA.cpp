#include "OnePlusLambdaCommaLambdaGA.h"

#include "CostFunction.h"
#include "OneMax.h"

#include <algorithm>
#include <chrono>
#include <math.h>

OnePlusLambdaCommaLambdaGA::OnePlusLambdaCommaLambdaGA(int N, CostFunction* aCostFunction, int aLambda) : GeneticAlgorithm(N, aCostFunction)
{
	mBitString = new int[mN];

	mLambda = aLambda;
	mP = (double)mLambda / (double)mN;
	mC = 1 / (double)mLambda;

	for(int i = 0; i < mN; ++i)
	{
		mInts.push_back(i);
	}

	CreateSamples();
}

OnePlusLambdaCommaLambdaGA::~OnePlusLambdaCommaLambdaGA()
{
	delete[] mBitString;
}

void OnePlusLambdaCommaLambdaGA::CreateSamples()
{
	for(int i = 1; i <= mN; ++i)
	{
		double over = 1;
		for(int j = 1; j <= i; ++j)
		{
			over *= (double)((double)mN - i + j) / (double)j;
		}
		if(i > 1)
		{
			mSamples.push_back(over * pow(mP, i) * pow(1 - mP, (double)mN - i)+mSamples.at(i-2));
		}
		else
		{
			mSamples.push_back(over * pow(mP, i) * pow(1 - mP, (double)mN - i));
		}
	}
}

int OnePlusLambdaCommaLambdaGA::SampleForL()
{
	std::uniform_real_distribution<> p(0.0, 1.0);
	double possibility = p(mRng);

	int l = 0;
	int r = mN - 1;
	while(l < r)
	{
		int m = floor((l + r) / 2);
		if(mSamples.at(m) < possibility)
		{
			l = m + 1;
		}
		else
		{
			r = m;
		}
	}
	return l;
}

void OnePlusLambdaCommaLambdaGA::RandomizeBitString()
{
	for(int i = 0; i < mN; ++i)
	{
		mBitString[i] = mCoin(mRng);
	}
}

std::vector<std::pair<int*, double>>* OnePlusLambdaCommaLambdaGA::CreateMutations()
{
	std::vector<std::pair<int*, double>>* mutations = new std::vector<std::pair<int*, double>>;

	for(int i = 0; i < mLambda; ++i)
	{
		int* bitStringPrime = new int[mN];
		std::copy(mBitString, mBitString + mN, bitStringPrime);

		int l = SampleForL();
		std::shuffle(std::begin(mInts), std::end(mInts), mRng);

		for(int j = 0; j < l; ++j)
		{
			if(bitStringPrime[mInts.at(j)] == 0)
			{
				bitStringPrime[mInts.at(j)] = 1;
			}
			else
			{
				bitStringPrime[mInts.at(j)] = 0;
			}
		}

		mutations->push_back(std::make_pair(bitStringPrime, mCostFunction->GetFitnessValue(bitStringPrime)));
	}

	return mutations;
}

std::vector<std::pair<int*, double>>* OnePlusLambdaCommaLambdaGA::CreateCrossOver(int* aBitStringPrime)
{
	std::vector<std::pair<int*, double>>* crossovers = new std::vector<std::pair<int*, double>>;

	std::uniform_real_distribution<> p(0.0, 1.0);

	for(int i = 0; i < mLambda; ++i)
	{
		int* y = new int[mN];

		for(int j = 0; j < mN; ++j)
		{
			if(p(mRng) > mC)
			{
				y[j] = mBitString[j];
			}
			else
			{
				y[j] = aBitStringPrime[j];
			}
		}
		crossovers->push_back(std::make_pair(y,mCostFunction->GetFitnessValue(y)));
	}

	delete[] aBitStringPrime;

	return crossovers;
}

std::pair<long long, double> OnePlusLambdaCommaLambdaGA::RunEA()
{
	RandomizeBitString();

	mFitnessValue = mCostFunction->GetFitnessValue(mBitString);
	
	long long iterations = 0;
	bool justUpdated = false;

	double maximumFitnessValue = mCostFunction->GetMaximumFitnessValue();

	auto start = std::chrono::steady_clock::now();

	while(!justUpdated || maximumFitnessValue != mFitnessValue)
	{
		iterations++;

		if(justUpdated)
		{
			justUpdated = false;
		}

		auto bitStringPrime = SelectBestDeleteRest(CreateCrossOver(SelectBestDeleteRest(CreateMutations()).first));

		if(mFitnessValue <= bitStringPrime.second)
		{
			mFitnessValue = bitStringPrime.second;

			delete[] mBitString;
			mBitString = bitStringPrime.first;
			
			justUpdated = true;
		}
		else
		{
			delete[] bitStringPrime.first;
		}
	}

	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsedSeconds = end - start;

	return std::make_pair(iterations, elapsedSeconds.count());
}