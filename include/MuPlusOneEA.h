#pragma once

#include "EvolutionaryAlgorithm.h"

#include <string>

class MuPlusOneEA : public EvolutionaryAlgorithm
{
public:
	MuPlusOneEA(int aN, CostFunction* aCostFunction, int aPopulation);
	~MuPlusOneEA();

	void CreateInitialPopulation();
	void DeletePopulation();

	std::pair<int*, double> CreateRandomBitStringFromParents();

	std::pair<long long, double> RunEA() override;
	std::string GetEAName() override { return "(" + std::to_string(mPopulation.size()) + " + 1) EA"; }
private:
	int mPopulationSize;
	std::vector<std::pair<int*, double>> mPopulation;
	
};

