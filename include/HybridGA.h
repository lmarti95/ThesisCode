#pragma once

#include "GeneticAlgorithm.h"

#include <string>

class HybridGA : public GeneticAlgorithm
{
public:
	HybridGA(int aN, CostFunction* aCostFunction);

	void SetUpPermutation();
	int* MajorityVoting(std::vector<int*> aParents);

	std::pair<long long, double> RunEA() override;
	std::string GetEAName() override { return "Hybrid GA"; }
private:
	std::vector<int> mPermutation;
};