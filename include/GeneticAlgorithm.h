#pragma once

#include "EvolutionaryAlgorithm.h"

class GeneticAlgorithm : public EvolutionaryAlgorithm
{
public:
	GeneticAlgorithm(int N, CostFunction* aCostFunction);

	virtual std::pair<long long, double> RunEA() = 0;
	virtual std::string GetEAName() = 0;
protected:
	int* CreateMutatedOffSpringUniformCrossover(std::vector<std::pair<int*, double>> aParents);
};

