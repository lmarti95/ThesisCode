#pragma once

#include "GeneticAlgorithm.h"

#include <mutex>
#include <string>

class MuPlusOneGA : public GeneticAlgorithm
{
public:
	MuPlusOneGA(int aN, CostFunction* aCostFunction, int aPopulation);
	~MuPlusOneGA();

	void CreateInitialPopulation();
	void DeletePopulation();

	std::pair<int*, double> CreateRandomBitStringFromParents();
	int* Mutate(int* aBitString);
	void DeleteWorst();

	std::pair<long long, double> RunEA() override;
	std::string GetEAName() override { return "(" + std::to_string(mPopulationSize) + " + 1) GA"; }
	std::vector<int>* GetBitString() override;

	void UpdateBitString();
private:
	int mPopulationSize;
	std::vector<std::pair<int*, double>> mPopulation;

	int mPc = 1;

#ifdef GRAPHICS
	std::mutex mBitStringMutex;
	int* mBitString;
#endif
};