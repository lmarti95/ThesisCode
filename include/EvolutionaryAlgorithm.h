#pragma once

#include "CostFunction.h"

#include <atomic>
#include <thread>
#include <random>

enum class StagnationDetection{NotApplicable, Off, On};

class EvolutionaryAlgorithm
{
public:
	EvolutionaryAlgorithm(int N, CostFunction* aCostFunction);
	~EvolutionaryAlgorithm();

	EvolutionaryAlgorithm(const EvolutionaryAlgorithm&) = delete;
	EvolutionaryAlgorithm& operator= (const EvolutionaryAlgorithm&) = delete;

	void CopyCostFunction(CostFunction* aCostFunction);
	CostFunction* GetCostFunction(){ return mCostFunction; }
	void SetCostFunction(CostFunction* aCostFunction) { mCostFunction = aCostFunction; }

	virtual std::pair<long long, double> RunEA() = 0;
	virtual std::string GetEAName() = 0;
	std::string GetCostFunctionName() { return mCostFunction->GetCostFunctionName(); }

#if GRAPHICS
	void SetDelay(int aDelay) { mDelay = aDelay; }
#endif

	double GetN() { return mN; }

	double GetFitnessValue() { return mFitnessValue; }
	long GetIterations() { return mIterations; }

	virtual std::vector<int>* GetBitString() = 0;

	StagnationDetection GetStagnationDetection() { return mStagnationDetection; }

protected:

	double FlipBits(int* aBitString, int aNext);
	long SavePosition(long aPos);
	long GetNextPosition(long aPos);

	std::pair<int*, double> SelectBestDeleteRest(std::vector<std::pair<int*, double>>* aMutations, int* aRRate = nullptr);
	
	int* CreateRandomBitString();
	inline int FlipBitBasedOnPosition(int* aBitString, int aPosition);

	int mN;

#if GRAPHICS
	std::atomic<double> mFitnessValue = 0;
	std::atomic_long mIterations = {0};
	int mDelay = 0;
#else
	double mFitnessValue = 0;
	long long mIterations = 0;
#endif
	

	long mNextPosition = -1;
	std::random_device mDev;
	std::mt19937 mRng;
	double mCalculatedLog;
	std::uniform_int_distribution<std::mt19937::result_type> mCoin;
	std::uniform_int_distribution<std::mt19937::result_type> mRandomN;

	CostFunction* mCostFunction;

	bool mFitnessChangePossible = false;

	StagnationDetection mStagnationDetection = StagnationDetection::NotApplicable;
};

inline int EvolutionaryAlgorithm::FlipBitBasedOnPosition(int* aBitString, int aPosition)
{
	if(aBitString[aPosition] == 1)
	{
		aBitString[aPosition] = 0;
		return -1;
	}
	else
	{
		aBitString[aPosition] = 1;
		return 1;
	}
}
