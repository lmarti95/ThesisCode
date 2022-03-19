#include "EvolutionaryAlgorithm.h"

#include "Cliff.h"
#include "Jump.h"
#include "MST.h"

#include<algorithm>

EvolutionaryAlgorithm::EvolutionaryAlgorithm(int aN, CostFunction* aCostFunction)
{
	mN = aN;
	mCalculatedLog = log(1.0 - 1.0 / mN);
	std::mt19937 tempRng(mDev());
	mRng = tempRng;
	std::uniform_int_distribution<std::mt19937::result_type> tempCoin(0,1);
	mCoin = tempCoin;
	std::uniform_int_distribution<std::mt19937::result_type> tempRandomN(1, mN);
	mRandomN = tempRandomN;

	if(dynamic_cast<Jump*>(mCostFunction) != nullptr || dynamic_cast<Cliff*>(mCostFunction) != nullptr)
	{
		mFitnessChangePossible = true;
	}

	CopyCostFunction(aCostFunction);
}

EvolutionaryAlgorithm::~EvolutionaryAlgorithm()
{
	delete mCostFunction;
}

void EvolutionaryAlgorithm::CopyCostFunction(CostFunction* aCostFunction)
{
	if(dynamic_cast<Jump*>(aCostFunction) != nullptr)
	{
		mCostFunction = new Jump(*dynamic_cast<Jump*>(aCostFunction));
	}
	if(dynamic_cast<Cliff*>(aCostFunction) != nullptr)
	{
		mCostFunction = new Cliff(*dynamic_cast<Cliff*>(aCostFunction));
	}
	if(dynamic_cast<MST*>(aCostFunction) != nullptr)
	{
		mCostFunction = new MST(*dynamic_cast<MST*>(aCostFunction));
	}
}

double EvolutionaryAlgorithm::FlipBits(int* aBitString, int aNext)
{
	double fitnessValueChange = 0;
	long start = 0;

	while(aNext != -1)
	{
		if(aBitString[start + aNext] == 0)
		{
			aBitString[start + aNext] = 1;
			fitnessValueChange++;
		}
		else
		{
			aBitString[start + aNext] = 0;
			fitnessValueChange--;
		}
		start = start + aNext + 1;
		aNext = GetNextPosition(mN - start - 1);
	}

	return fitnessValueChange;
}

long EvolutionaryAlgorithm::SavePosition(long aPos)
{
	if(mNextPosition > aPos)
	{
		mNextPosition = mNextPosition - aPos - 1;
		return -1;
	}
	else
	{
		long next = mNextPosition;
		mNextPosition = -1;
		return next;
	}
}

long EvolutionaryAlgorithm::GetNextPosition(long aPos)
{
	std::uniform_real_distribution<> p(0.0, 1.0);
	if(mNextPosition == -1)
	{
		auto x = p(mRng);
		mNextPosition = (long)floor(log(x) / mCalculatedLog);
	}

	return SavePosition(aPos);
}

std::pair<int*, double> EvolutionaryAlgorithm::SelectBestDeleteRest(std::vector<std::pair<int*, double>>* aMutations, int* aRRate)
{
	double highestFitnesssValue = 0;
	for(auto& offspring : *aMutations)
	{
		if(highestFitnesssValue < offspring.second)
		{
			highestFitnesssValue = offspring.second;
		}
	}

	int bestOnes = 0;

	for(auto& offspring : *aMutations)
	{
		if(highestFitnesssValue == offspring.second)
		{
			bestOnes++;
		}
	}

	std::uniform_int_distribution<std::mt19937::result_type> selectOffspring(0, bestOnes-1);
	int select = selectOffspring(mRng);

	int* bitString = nullptr;
	double fitnessValue = 0;

	int bestCount = 0;
	for(unsigned int i = 0; i < aMutations->size(); ++i)
	{
		if(highestFitnesssValue == aMutations->at(i).second)
		{
			if(select == bestCount)
			{
				bitString = aMutations->at(i).first;
				fitnessValue = aMutations->at(i).second;
				bestCount++;
				if(aRRate)
				{
					if(i <= aMutations->size() / 2)
					{
						*aRRate = 0;
					}
					else
					{
						*aRRate = 2;
					}
				}
				continue;
			}
			else
			{
				bestCount++;
			}
		}

		delete[] aMutations->at(i).first;
	}

	delete aMutations;

	return std::make_pair(bitString, fitnessValue);
}

int* EvolutionaryAlgorithm::CreateRandomBitString()
{
	int* bitString = new int[mN];

	for(int i = 0; i < mN; ++i)
	{
		bitString[i] = mCoin(mRng);
	}

	return bitString;
}
