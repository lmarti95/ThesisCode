#pragma once

#include "CostFunction.h"

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

	void ReadIn(std::string aFilename);

	double GetMaximumFitnessValue() override;
	double GetFitnessValue(int* aBitString) override;
	double GetFitnessValue(int aChange) { return -1; };
	void ApplyChange(int aChange) override {}
	std::string GetCostFunctionName() override { return "MST"; }

	void CalculateSum(int* aBitString) override{}
	double GetSum() override { return -1; }

	void visitEdge(int* aBitString, int aEdge, int* aConnected);
	int ConnectedComponents(int* aBitString);
private:
	int mNodesNum;
	int mEdgesNum;
	int mMinimumSum;
	std::vector<Edge> mEdges;
	int mM = 10000;
};