#pragma once

#include "CostFunction.h"
#include "EvolutionaryAlgorithm.h"

class SD_RLS_m : public EvolutionaryAlgorithm
{
public:
	SD_RLS_m(int aN, CostFunction* aCostFunction);
	~SD_RLS_m();

	void SetEpsilon(int aEpsilon);
	void RandomizeBitString();

	std::pair<long long, double> RunEA() override;
	std::string GetEAName() override { return "SD-RLS^m"; }
private:
	int* mBitString;
	int mR;
	int mEpsilon = 1;
};