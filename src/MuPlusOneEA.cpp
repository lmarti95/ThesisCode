#include "MuPlusOneEA.h"

#include <algorithm>
#include "CostFunction.h"
#include "OneMax.h"

#include <chrono>

MuPlusOneEA::MuPlusOneEA(int aN, CostFunction* aCostFunction, int aPopulation) : EvolutionaryAlgorithm(aN, aCostFunction)
{
	mPopulationSize = aPopulation;
}

MuPlusOneEA::~MuPlusOneEA()
{
	DeletePopulation();
}

void MuPlusOneEA::CreateInitialPopulation()
{
	DeletePopulation();

	for(int i = 0; i < mPopulationSize; ++i)
	{
		int* bitString = new int[mN];
		for(int i = 0; i < mN; ++i)
		{
			bitString[i] = mCoin(mRng);
		}
		double fitnessValue = mCostFunction->GetFitnessValue(bitString);
		mPopulation.push_back(std::make_pair(bitString, fitnessValue));
	}
}

void MuPlusOneEA::DeletePopulation()
{
	for(unsigned int i = 0; i < mPopulation.size(); ++i)
	{
		delete[] mPopulation.at(i).first;
	}

	mPopulation.clear();
}

std::pair<int*, double> MuPlusOneEA::CreateRandomBitStringFromParents()
{
	int* bitString = new int[mN];

	std::uniform_int_distribution<std::mt19937::result_type> selectParent(0, mPopulationSize - 1);

	auto selection = selectParent(mRng);

	std::copy(mPopulation.at(selection).first, mPopulation.at(selection).first + mN, bitString);

	return std::make_pair(bitString, mPopulation.at(selection).second);
}

std::pair<long long, double> MuPlusOneEA::RunEA()
{
	CreateInitialPopulation();

	int* bitStringPrime;

	double newFitnessValue = 0;
	long long iterations = 0;
	bool justUpdated = false;

	double maximumFitnessValue = mCostFunction->GetMaximumFitnessValue();

	auto start = std::chrono::steady_clock::now();

	while(!justUpdated || maximumFitnessValue != std::max_element(mPopulation.begin(), mPopulation.end(), [](auto a, auto b)
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
			auto pair  = CreateRandomBitStringFromParents();
			bitStringPrime = pair.first;
			newFitnessValue = pair.second;
			newFitnessValue += FlipBits(bitStringPrime, next);
		}
		else
		{
			continue;
		}

		if(dynamic_cast<OneMax*>(mCostFunction) == nullptr)
		{
			newFitnessValue = mCostFunction->GetFitnessValue(bitStringPrime);
		}

		auto min = std::min_element(mPopulation.begin(), mPopulation.end(), [](auto a, auto b)
			{
				return a.second < b.second;
			});

		if(newFitnessValue > min->second)
		{
			delete[] min->first;
			mPopulation.erase(min);
			mPopulation.push_back(std::make_pair(bitStringPrime, newFitnessValue));

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
