#pragma once

#include "GeneticAlgorithm.h"

#include <mutex>
#include <string>

class HybridGA : public GeneticAlgorithm
{
public:
	HybridGA(int aN, CostFunction* aCostFunction);

	void SetUpPermutation();
	int* MajorityVoting(std::vector<int*> aParents);

	std::pair<long long, double> RunEA() override;
	std::string GetEAName() override { return "Hybrid GA"; }
	std::vector<int>* GetBitString() override;
private:
	std::vector<int> mPermutation;

#if GRAPHICS
	std::mutex mBitStringMutex;
	int* mBitString;
#endif
};