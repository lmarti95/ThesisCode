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
#if GRAPHICS
	std::vector<int>* GetBitString();
#endif
protected:
	int* mBitString;
	const double mR;
	int mEpsilon = 1;

#if GRAPHICS
	std::mutex mBitStringMutex;
#endif
};
