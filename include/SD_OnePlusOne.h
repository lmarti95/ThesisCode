#pragma once

#include "CostFunction.h"
#include "SD.h"

class SD_OnePlusOne : public SD
{
public:
	SD_OnePlusOne(int aN, CostFunction* aCostFunction);
	~SD_OnePlusOne();

	bool CalculateFlipR(double aR);

	std::pair<long long, double> RunEA() override;
	std::string GetEAName() override { return "SD-(1+1)"; }
private:
};