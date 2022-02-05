#pragma once

#include "CostFunction.h"
#include "SD.h"

class SD_RLS_STAR : public SD
{
public:
	SD_RLS_STAR(int aN, CostFunction* aCostFunction);
	~SD_RLS_STAR();

	std::pair<long long, double> RunEA() override;
	std::string GetEAName() override { return "SD-RLS-STAR"; }
private:
};