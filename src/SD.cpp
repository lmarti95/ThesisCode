#include "SD.h"

SD::SD(int aN, CostFunction* aCostFunction) : EvolutionaryAlgorithm(aN, aCostFunction), mR(std::pow(mN, 3 + mEpsilon))
{
	mBitString = new int[mN];

#if GRAPHICS
	for(int i = 0; i < mN; ++i)
	{
		mBitString[i] = 0;
	}
#endif

	mStagnationDetection = StagnationDetection::Off;
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

#if GRAPHICS
std::vector<int>* SD::GetBitString()
{
	std::lock_guard<std::mutex> lg{mBitStringMutex};
	std::vector<int>* bitString = new std::vector<int>;

	for(int i = 0; i < mN; ++i)
	{
		bitString->push_back(mBitString[i]);
	}

	return bitString;
}
#endif
