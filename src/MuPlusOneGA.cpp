#include "MuPlusOneGA.h"

#include "CostFunction.h"

#include <algorithm>
#include <chrono>

MuPlusOneGA::MuPlusOneGA(int aN, CostFunction* aCostFunction, int aPopulation) : GeneticAlgorithm(aN, aCostFunction)
{
	mPopulationSize = aPopulation;
}

MuPlusOneGA::~MuPlusOneGA()
{
	DeletePopulation();
}

void MuPlusOneGA::CreateInitialPopulation()
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

void MuPlusOneGA::DeletePopulation()
{
	for(unsigned int i = 0; i < mPopulation.size(); ++i)
	{
		delete[] mPopulation.at(i).first;
	}

	mPopulation.clear();
}

std::pair<int*, double> MuPlusOneGA::CreateRandomBitStringFromParents()
{
	int* bitString = new int[mN];

	std::uniform_int_distribution<std::mt19937::result_type> selectParent(0, mPopulationSize - 1);

	auto selection = selectParent(mRng);

	std::copy(mPopulation.at(selection).first, mPopulation.at(selection).first + mN, bitString);

	return std::make_pair(bitString, mPopulation.at(selection).second);
}

int* MuPlusOneGA::Mutate(int* aBitString)
{
	long next = GetNextPosition(mN - 1);
	if(next != -1)
	{
		FlipBits(aBitString, next);
	}

	return aBitString;
}

void MuPlusOneGA::DeleteWorst()
{
	double worstFitnessValue = mPopulation.at(0).second;

	for(auto& p : mPopulation)
	{
		if(worstFitnessValue > p.second)
		{
			worstFitnessValue = p.second;
		}
	}

	int worstOnes = 0;

	for(auto& p : mPopulation)
	{
		if(worstFitnessValue == p.second)
		{
			worstOnes++;
		}
	}

	std::uniform_int_distribution<std::mt19937::result_type> selectP(0, worstOnes - 1);
	int select = selectP(mRng);
	int counter = 0;

	for(unsigned int i = 0; i < mPopulation.size(); ++i)
	{
		if(mPopulation.at(i).second == worstFitnessValue)
		{
			if(counter == select)
			{
				delete[] mPopulation.at(i).first;
				mPopulation.erase(mPopulation.begin() + i);
				return;
			}
			else
			{
				counter++;
			}
		}
	}
}

std::pair<long long, double> MuPlusOneGA::RunEA()
{
	CreateInitialPopulation();

	long long iterations = 0;

	double maximumFitnessValue = mCostFunction->GetMaximumFitnessValue();

	std::uniform_real_distribution<> p(0.0, 1.0);
	std::uniform_int_distribution<std::mt19937::result_type> parentsRandom(0, mPopulationSize-1);

	auto start = std::chrono::steady_clock::now();

	while(maximumFitnessValue != std::max_element(mPopulation.begin(), mPopulation.end(), [](auto a, auto b)
		{
			return a.second < b.second;
		})->second)
	{
		iterations++;

		if(mPc == 1 || p(mRng) < mPc)
		{
			int parent1 = parentsRandom(mRng);
			int parent2 = parentsRandom(mRng);
			while(parent1 == parent2)
			{
				parent2 = parentsRandom(mRng);
			}

			std::vector<std::pair<int*, double>> parents;
			parents.push_back(mPopulation.at(parent1));
			parents.push_back(mPopulation.at(parent2));

			int* offspring = Mutate(CreateMutatedOffSpringUniformCrossover(parents));

			mPopulation.push_back(std::make_pair(offspring, mCostFunction->GetFitnessValue(offspring)));
		}
		else
		{
			int parent = parentsRandom(mRng);
			int* offspring = new int[mN];
			std::copy(mPopulation.at(parent).first, mPopulation.at(parent).first + mN, offspring);
			Mutate(offspring);

			mPopulation.push_back(std::make_pair(offspring, mCostFunction->GetFitnessValue(offspring)));
		}

		DeleteWorst();
	}

	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsedSeconds = end - start;

	return std::make_pair(iterations, elapsedSeconds.count());
}
