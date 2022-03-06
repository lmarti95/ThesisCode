#include "HybridGA.h"

#include "CostFunction.h"

#include <algorithm>
#include <chrono>

HybridGA::HybridGA(int aN, CostFunction* aCostFunction) : GeneticAlgorithm(aN, aCostFunction)
{
	SetUpPermutation();
}

void HybridGA::SetUpPermutation()
{
	for(int i = 0; i < mN; ++i)
	{
		mPermutation.push_back(i);
	}
}

int* HybridGA::MajorityVoting(std::vector<int*> aParents)
{
	int* offspring = new int[mN];
	
	for(int i = 0; i < mN; ++i)
	{
		unsigned int ones = 0;
		for(unsigned int j = 0; j < aParents.size(); ++j)
		{
			if(aParents.at(j)[i] == 1)
			{
				ones++;
			}
		}

		if(ones > aParents.size() / 2)
		{
			offspring[i] = 1;
		}
		else
		{
			offspring[i] = 0;
		}
	}

	return offspring;
}

std::pair<long long, double> HybridGA::RunEA()
{
	long long iterations = 0;

	double maximumFitnessValue = mCostFunction->GetMaximumFitnessValue();

	double fitnessValue = 0;
	std::vector<int*> parents;

	auto start = std::chrono::steady_clock::now();

	while(maximumFitnessValue != fitnessValue)
	{
		for(int i = 0; i < 3; ++i)
		{
			int* bitString = CreateRandomBitString();
			fitnessValue = mCostFunction->GetFitnessValue(bitString);
			bool updated = true;

			while(updated)
			{
				updated = false;
				std::shuffle(mPermutation.begin(), mPermutation.end(), mRng);

				for(auto& position : mPermutation)
				{
					iterations++;
					FlipBitBasedOnPosition(bitString, position);

					int newFitnessValue = mCostFunction->GetFitnessValue(bitString);
					if(newFitnessValue > fitnessValue)
					{
						fitnessValue = newFitnessValue;
						updated = true;
					}
					else
					{
						FlipBitBasedOnPosition(bitString, position);
					}
				}
			}

			parents.push_back(bitString);
		}

		int* offspring = MajorityVoting(parents);

		iterations++;
		fitnessValue = mCostFunction->GetFitnessValue(offspring);
		
		delete[] offspring;
		for(auto& p : parents)
		{
			delete[] p;
		}
		parents.clear();
	}

	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsedSeconds = end - start;

	return std::make_pair(iterations, elapsedSeconds.count());
}
