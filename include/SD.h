#pragma once

#include "EvolutionaryAlgorithm.h"

class SD : public EvolutionaryAlgorithm
{
public:
	SD(int aN, CostFunction* aCostFunction);
	~SD();

	void SetEpsilon(int aEpsilon);
	void RandomizeBitString();
protected:
	int* mBitString;
	int mR;
	int mEpsilon = 1;
};