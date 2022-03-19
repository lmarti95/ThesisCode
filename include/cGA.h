#pragma once

#include <mutex>

#include "GeneticAlgorithm.h"

class cGA : public GeneticAlgorithm
{
public:
	cGA(int N, CostFunction* aCostFunction, int aMu);
	~cGA();

	std::pair<long long, double> RunEA() override;
	std::string GetEAName() override { return "cGA(" + std::to_string(mMu) + ")"; }
	std::vector<int>* GetBitString() override;

	void SetF();
	int* Sample();
private:
	int mMu;
	double* mF;

#ifdef GRAPHICS
	std::mutex mBitStringMutex;
	int* mBitString;
#endif
};