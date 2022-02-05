#include "SD.h"

SD::SD(int aN, CostFunction* aCostFunction) : EvolutionaryAlgorithm(aN, aCostFunction)
{
	mBitString = new int[mN];
	mR = std::pow(mN, 3 + mEpsilon);
}

SD::~SD()
{
	delete[] mBitString;
}

void SD::SetEpsilon(int aEpsilon)
{
	mEpsilon = aEpsilon;
}

void SD::RandomizeBitString()
{
	for(int i = 0; i < mN; ++i)
	{
		mBitString[i] = mCoin(mRng);
	}
}