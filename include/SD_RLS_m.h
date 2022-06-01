#pragma once

#include "CostFunction.h"
#include "SD.h"

class SD_RLS_m : public SD
{
public:
	SD_RLS_m(int aN, CostFunction* aCostFunction);
	~SD_RLS_m() override;

	std::pair<long long, double> RunEA() override;
	std::string GetEAName() override { return "SD-RLS_m"; }
private:
};
