#pragma once

#include "CostFunction.h"
#include "EvolutionaryAlgorithm.h"

class SASD_OnePlusLambda : public EvolutionaryAlgorithm
{
public:
	SASD_OnePlusLambda(int aN, CostFunction* aCostFunction, int aLambda);
	~SASD_OnePlusLambda();

	void RandomizeBitString();
	bool CalculateFlipR();
	std::vector<std::pair<int*, double>>* CreateOffsprings();

	std::pair<long long, double> RunEA() override;
	std::string GetEAName() override { return "SASD-(1+" + std::to_string(mLambda) + ")"; }
private:
	int mLambda;
	int* mBitString;
	int mR;
	int mNumeratorMultiplier = 1;
	int mDenominatorMultiplier = 1;
	int mLastRRate = -1;
};