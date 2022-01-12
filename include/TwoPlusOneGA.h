#pragma once

#include "GeneticAlgorithm.h"

class TwoPlusOneGA : public GeneticAlgorithm
{
public:
	TwoPlusOneGA(int N, CostFunction* aCostFunction);
	~TwoPlusOneGA();

	void CreateParents();
	void DeleteParents();

	std::pair<long long, double> RunEA() override;
	std::string GetEAName() override { return "(2 + 1) GA"; }
private:
	std::vector<std::pair<int*, double>> mParents;
};

