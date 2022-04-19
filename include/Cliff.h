#pragma once

#include "CostFunction.h"

#include <atomic>

class Cliff : public CostFunction
{
public:
	Cliff(int aN);

	Cliff(const Cliff& aOld);

	double GetMaximumFitnessValue() override;
	double GetFitnessValue(int* aBitString) override;
	double GetFitnessValue(int aChange) override;
	void ApplyChange(int aChange) override { mSum += aChange; }
	std::string GetCostFunctionName() override { return "Cliff"; }

	void CalculateSum(int* aBitString) override;

	void SetSum(double aSum) { mSum = aSum; }
	double GetSum() override { return mSum; }

	double FitnessValueToSum(double aFitness) override;

private:
#if GRAPHICS
	std::atomic_long mSum = {0};
#else
	double mSum = 0;
#endif
};