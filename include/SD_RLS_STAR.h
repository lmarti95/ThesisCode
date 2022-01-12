#pragma once

#include "CostFunction.h"
#include "EvolutionaryAlgorithm.h"

class SD_RLS_STAR : public EvolutionaryAlgorithm
{
public:
	SD_RLS_STAR(int aN, CostFunction* aCostFunction);
	~SD_RLS_STAR();

	void SetEpsilon(int aEpsilon);
	void RandomizeBitString();

	std::pair<long long, double> RunEA() override;
	std::string GetEAName() override { return "SD-RLS"; }
private:
	int* mBitString;
	long long mR;
	int mEpsilon = 1;
};