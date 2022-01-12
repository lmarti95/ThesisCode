#pragma once
#include "CostFunction.h"

class LeadingOnes : public CostFunction
{
public:
	LeadingOnes(int aN);

	double GetMaximumFitnessValue() override;
	double GetFitnessValue(int* aBitString) override;
	std::string GetCostFunctionName() override { return "LeadingOnes"; }
};
