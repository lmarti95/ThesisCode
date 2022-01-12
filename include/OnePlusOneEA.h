#pragma once

#include "CostFunction.h"
#include "EvolutionaryAlgorithm.h"

enum class OnePlusOneEAOptimization{No, FitnessEvaluation, RandomNumberGeneration, All};

class OnePlusOneEA : public EvolutionaryAlgorithm
{
public:
	OnePlusOneEA(int aN, CostFunction* aCostFunction);
	~OnePlusOneEA();

	void SetOptimizationLevel(OnePlusOneEAOptimization aOpt) { mOptimizationLevel = aOpt; }
	OnePlusOneEAOptimization GetOptimizationLevel() { return mOptimizationLevel; }

	void RandomizeBitString();

	std::pair<long long, double> RunEA() override;
	std::string GetEAName() override { return "(1 + 1) EA";}

	std::pair<long long, double> NoOptimizationOneOne();
	std::pair<long long, double> FitnessEvaluationOptimizationOneOne();
	std::pair<long long, double> RandomNumberGenerationOptimizationOneOne();
	std::pair<long long, double> AllOptimizationOneOne();
private:
	OnePlusOneEAOptimization mOptimizationLevel;

	int* mBitString;
};

