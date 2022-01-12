#pragma once

#include "EvolutionaryAlgorithm.h"

class OneCommaLambdaEA : public EvolutionaryAlgorithm
{
public:
	OneCommaLambdaEA(int N, CostFunction* aCostFunction, int aLambda);
	~OneCommaLambdaEA();

	void RandomizeBitString();
	std::vector<std::pair<int*, double>>* CreateOffsprings();

	std::pair<long long, double> RunEA() override;
	std::string GetEAName() override { return "(1," + std::to_string(mLambda) +") EA"; }
private:
	int* mBitString;

	double mFitnessValue;

	int mLambda;
};

