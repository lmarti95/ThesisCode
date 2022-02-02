#pragma once

#include "CostFunction.h"
#include "EvolutionaryAlgorithm.h"

class SD_RLS_r : public EvolutionaryAlgorithm
{
public:
	SD_RLS_r(int aN, CostFunction* aCostFunction);
	~SD_RLS_r();

	void SetEpsilon(int aEpsilon);
	void RandomizeBitString();

	std::pair<long long, double> RunEA() override;
	std::string GetEAName() override { return "SD-RLS^r"; }
private:
	int* mBitString;
	int mR;
	int mEpsilon = 1;
};