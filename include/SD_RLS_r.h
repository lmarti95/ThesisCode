#pragma once

#include "CostFunction.h"
#include "SD.h"

class SD_RLS_r : public SD
{
public:
	SD_RLS_r(int aN, CostFunction* aCostFunction);
	~SD_RLS_r();

	std::pair<long long, double> RunEA() override;
	std::string GetEAName() override { return "SD-RLS^r"; }
private:
};