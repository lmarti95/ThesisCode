#pragma once

#include "CostFunction.h"

class Cliff : public CostFunction
{
public:
	Cliff(int aN);

	double GetMaximumFitnessValue() override;
	double GetFitnessValue(int* aBitString) override;
	std::string GetCostFunctionName() override { return "Cliff"; }
};