#pragma once

#include "CostFunction.h"

class Jump : public CostFunction
{
public:
	Jump(int aN, int aGapSize);

	double GetMaximumFitnessValue() override;
	double GetFitnessValue(int* aBitString) override;
	std::string GetCostFunctionName() override { return "Jump(" +std::to_string(mGapSize) + ")" ; }
private:
	int mGapSize;
};
