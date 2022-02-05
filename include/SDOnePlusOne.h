#pragma once

#include "CostFunction.h"
#include "SD.h"

class SDOnePlusOne : public SD
{
public:
	SDOnePlusOne(int aN, CostFunction* aCostFunction);
	~SDOnePlusOne();

	bool CalculateFlipR();

	std::pair<long long, double> RunEA() override;
	std::string GetEAName() override { return "SD-(1+1)"; }
private:
};