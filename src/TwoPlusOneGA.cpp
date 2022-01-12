#include "TwoPlusOneGA.h"

#include <algorithm>
#include "CostFunction.h"
#include "OneMax.h"

#include <chrono>

TwoPlusOneGA::TwoPlusOneGA(int N, CostFunction* aCostFunction) : GeneticAlgorithm(N, aCostFunction)
{
}

TwoPlusOneGA::~TwoPlusOneGA()
{
	DeleteParents();
}

void TwoPlusOneGA::CreateParents()
{
	DeleteParents();

	for(int i = 0; i < 2; ++i)
	{
		int* bitString = new int[mN];
		for(int i = 0; i < mN; ++i)
		{
			bitString[i] = mCoin(mRng);
		}
		double fitnessValue = mCostFunction->GetFitnessValue(bitString);
		mParents.push_back(std::make_pair(bitString, fitnessValue));
	}
}

void TwoPlusOneGA::DeleteParents()
{
	for(unsigned int i = 0; i < mParents.size(); ++i)
	{
		delete[] mParents.at(i).first;
	}

	mParents.clear();
}

std::pair<long long, double> TwoPlusOneGA::RunEA()
{
	CreateParents();

	int* bitStringPrime;

	double newFitnessValue = 0;
	long long iterations = 0;
	bool justUpdated = false;

	double maximumFitnessValue = mCostFunction->GetMaximumFitnessValue();

	auto start = std::chrono::steady_clock::now();

	while(!justUpdated || maximumFitnessValue != std::max_element(mParents.begin(), mParents.end(), [](auto a, auto b)
		{
			return a.second < b.second;
		})->second)
	{
		iterations++;

		if(justUpdated)
		{
			justUpdated = false;
		}

		long next = GetNextPosition(mN - 1);
		if(next != -1)
		{
			bitStringPrime = CreateMutatedOffSpringUniformCrossover(mParents);
			FlipBits(bitStringPrime, next);
		}
		else
		{
			continue;
		}

		newFitnessValue = mCostFunction->GetFitnessValue(bitStringPrime);

		auto min = std::min_element(mParents.begin(), mParents.end(), [](auto a, auto b)
		{
			return a.second < b.second;
		});

		if(newFitnessValue > min->second)
		{
			delete[] min->first;
			mParents.erase(min);
			mParents.push_back(std::make_pair(bitStringPrime, newFitnessValue));

			justUpdated = true;
		}
		else
		{
			delete[] bitStringPrime;
		}
	}

	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsedSeconds = end - start;

	return std::make_pair(iterations, elapsedSeconds.count());
}
