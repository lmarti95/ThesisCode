#pragma once

#include "CostFunction.h"

class Cliff : public CostFunction
{
public:
	Cliff(int aN);

	double GetMaximumFitnessValue() override;
	double GetFitnessValue(int* aBitString) override;
	double GetFitnessValue(int aChange) override;
	void ApplyChange(int aChange) override { mSum += aChange; }
	std::string GetCostFunctionName() override { return "Cliff"; }

	void SetSum(double aSum) { mSum = aSum; }
private:
	double mSum;
};