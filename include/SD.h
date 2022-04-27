#pragma once

#include "EvolutionaryAlgorithm.h"

#include <mutex>

class SD : public EvolutionaryAlgorithm
{
public:
	SD(int aN, CostFunction* aCostFunction);
	~SD();

	void SetEpsilon(int aEpsilon);
	void RandomizeBitString();
	std::vector<int>* GetBitString();
protected:
	int* mBitString;
	const double mR;
	int mEpsilon = 1;

#if GRAPHICS
	std::mutex mBitStringMutex;
#endif
};