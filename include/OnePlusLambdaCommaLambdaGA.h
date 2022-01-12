#pragma once

#include "GeneticAlgorithm.h"

class OnePlusLambdaCommaLambdaGA : public GeneticAlgorithm
{
public:
	OnePlusLambdaCommaLambdaGA(int N, CostFunction* aCostFunction, int aLambda);
	~OnePlusLambdaCommaLambdaGA();

	void CreateSamples();
	int SampleForL();

	void RandomizeBitString();

	std::vector<std::pair<int*, double>>* CreateMutations();
	std::vector<std::pair<int*, double>>* CreateCrossOver(int* aBitStringPrime);

	std::pair<long long, double> RunEA() override;
	std::string GetEAName() override { return "(1 + (" + std::to_string(mLambda) +  "," + std::to_string(mLambda) +")) GA"; }
private:
	int* mBitString;
	int mLambda;
	std::vector<double> mSamples;

	double mP;
	double mC;

	double mFitnessValue;

	std::vector<int> mInts;
};

