#pragma once

#include "CostFunction.h"

#include <cmath>
#include <mutex>
#include <vector>

struct Edge{
	int n1;
	int n2;
	int weight;
};

class MST : public CostFunction
{
public:
	MST(int aN, std::string aFilename);
	~MST() override{};

	void ReadIn(std::string aFilename);

	double GetMaximumFitnessValue() override;
	double GetFitnessValue(int* aBitString) override;
	double GetFitnessValue(int aChange) { return (double)-1*std::pow(mM, 4); };
	void ApplyChange(int aChange) override {}
	std::string GetCostFunctionName() override { return "MST(" + std::to_string(mNodesNum) + ")"; }

	void CalculateSum(int* aBitString) override{}
	double GetSum() override { return (double)-1 * std::pow(mM, 4); }

	void VisitEdge(int* aBitString, int aEdge, int* aConnected);
	int ConnectedComponents(int* aBitString);

	int GetNodesNum() { return mNodesNum; }
	int GetEdgesNum() { return mEdgesNum; }
	int GetMinimumSum() { return mMinimumSum; }
	std::vector<Edge>* GetEdges() { std::lock_guard<std::mutex> lg{mGetEdgeMutex}; return &mEdges; }

	double FitnessValueToSum(double aFitness) override { return (double)-1 * std::pow(mM, 4); }

	static std::mutex mGetEdgeMutex;

private:
	int mNodesNum;
	int mEdgesNum;
	int mMinimumSum;
	std::vector<Edge> mEdges;
	int mM = 10000;
};
