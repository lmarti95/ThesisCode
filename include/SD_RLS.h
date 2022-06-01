#pragma once

#include "CostFunction.h"
#include "SD.h"

class SD_RLS : public SD
{
public:
	SD_RLS(int aN, CostFunction* aCostFunction);
	~SD_RLS() override;

	std::pair<long long, double> RunEA() override;
	std::string GetEAName() override { return "SD-RLS"; }
private:
};
