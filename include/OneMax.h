#pragma once

#include "CostFunction.h"

class OneMax : public CostFunction
{
public:
	OneMax(int aN);

	double GetMaximumFitnessValue() override;
	double GetFitnessValue(int* aBitString) override;
	std::string GetCostFunctionName() override { return "OneMax"; }
};

