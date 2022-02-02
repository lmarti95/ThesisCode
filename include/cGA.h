#pragma once

#include "GeneticAlgorithm.h"

class cGA : public GeneticAlgorithm
{
public:
	cGA(int N, CostFunction* aCostFunction, int aMu);
	~cGA();

	std::pair<long long, double> RunEA() override;
	std::string GetEAName() override { return "cGA with hypothetical population size: " + std::to_string(mMu); }

	void SetF();
	int* Sample();

private:
	int mMu;
	double* mF;
};