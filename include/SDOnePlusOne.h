#pragma once

#include "CostFunction.h"
#include "EvolutionaryAlgorithm.h"

class SDOnePlusOne : public EvolutionaryAlgorithm
{
public:
	SDOnePlusOne(int aN, CostFunction* aCostFunction);
	~SDOnePlusOne();

	void RandomizeBitString();
	bool CalculateFlipR();

	std::pair<long long, double> RunEA() override;
	std::string GetEAName() override { return "SD-(1+1)"; }
private:
	int* mBitString;
	int mR;
};