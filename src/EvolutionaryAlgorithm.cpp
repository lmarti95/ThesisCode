#include "EvolutionaryAlgorithm.h"

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

	mCostFunction = aCostFunction;
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

std::pair<int*, double> EvolutionaryAlgorithm::SelectBestDeleteRest(std::vector<std::pair<int*, double>>* aMutations)
{
	int position = std::distance(aMutations->begin(), std::max_element(aMutations->begin(), aMutations->end(), [](auto a, auto b)
	{
		return a.second < b.second;
	}));

	int* bitString = aMutations->at(position).first;
	double fitnessValue = aMutations->at(position).second;

	for(unsigned int i = 0; i < aMutations->size(); ++i)
	{
		if(i != position)
		{
			delete[] aMutations->at(i).first;
		}
	}

	delete aMutations;

	return std::make_pair(bitString, fitnessValue);
}