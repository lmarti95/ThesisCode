#pragma once

#include "CostFunction.h"
#include "EvolutionaryAlgorithm.h"

class SD_RLS : public EvolutionaryAlgorithm
{
public:
	SD_RLS(int aN, CostFunction* aCostFunction);
	~SD_RLS();

	void SetEpsilon(int aEpsilon);
	void RandomizeBitString();

	std::pair<long long, double> RunEA() override;
	std::string GetEAName() override { return "SD-RLS"; }
private:
	int* mBitString;
	int mR;
	int mEpsilon = 1;
};