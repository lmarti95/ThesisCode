#pragma once

#include "CostFunction.h"

#include <random>

class EvolutionaryAlgorithm
{
public:
	EvolutionaryAlgorithm(int N, CostFunction* aCostFunction);

	EvolutionaryAlgorithm(const EvolutionaryAlgorithm&) = delete;
	EvolutionaryAlgorithm& operator= (const EvolutionaryAlgorithm&) = delete;

	CostFunction* GetCostFunction(){ return mCostFunction; }
	void SetCostFunction(CostFunction* aCostFunction) { mCostFunction = aCostFunction; }

	virtual std::pair<long long, double> RunEA() = 0;
	virtual std::string GetEAName() = 0;
	std::string GetCostFunctionName() { return mCostFunction->GetCostFunctionName(); }

	int GetN() { return mN; }

protected:

	double FlipBits(int* aBitString, int aNext);
	long SavePosition(long aPos);
	long GetNextPosition(long aPos);

	std::pair<int*, double> SelectBestDeleteRest(std::vector<std::pair<int*, double>>* aMutations);

	int mN;

	long mNextPosition = -1;
	std::random_device mDev;
	std::mt19937 mRng;
	double mCalculatedLog;
	std::uniform_int_distribution<std::mt19937::result_type> mCoin;
	std::uniform_int_distribution<std::mt19937::result_type> mRandomN;

	CostFunction* mCostFunction;
};

