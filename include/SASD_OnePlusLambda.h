#pragma once

#include "CostFunction.h"
#include "SD.h"

class SASD_OnePlusLambda : public SD
{
public:
	SASD_OnePlusLambda(int aN, CostFunction* aCostFunction, int aLambda);
	~SASD_OnePlusLambda();

	bool CalculateFlipR(double aR);
	std::vector<std::pair<int*, double>>* CreateOffsprings(double aR);

	std::pair<long long, double> RunEA() override;
	std::string GetEAName() override { return "SASD-(1+" + std::to_string(mLambda) + ")"; }
private:
	int mLambda;
	int mNumeratorMultiplier = 1;
	int mDenominatorMultiplier = 1;
	int mLastRRate = -1;
};