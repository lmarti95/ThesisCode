#include "Jump.h"

#include <iostream>

Jump::Jump(int aN, int aGapSize, JumpType aType) : CostFunction(aN)
{
	mGapSize = aGapSize;
	mType = aType;

	CheckGapSize();
}

double Jump::GetMaximumFitnessValue()
{
	if(mType == JumpType::OffsetSpike)
	{
		return mN + mGapSize + 1;
	}

	return mN + mGapSize;
}

double Jump::GetFitnessValue(int* aBitString)
{
	switch(mType)
	{
	case JumpType::Original:
		return GetOriginalFitnessValue(aBitString);
	case JumpType::Offset:
		return GetOffsetFitnessValue(aBitString);
	case JumpType::OffsetSpike:
		return GetOffsetSpikeFitnessValue(aBitString);
	default:
		return GetOriginalFitnessValue(aBitString);
	}
}

double Jump::GetOriginalFitnessValue(int* aBitString)
{
	double sum = 0;

	for(int i = 0; i < mN; ++i)
	{
		sum += aBitString[i];
	}

	if(sum > mN - mGapSize && sum != mN)
	{
		return  mN - sum;
	}

	return sum + mGapSize;
}

double Jump::GetOffsetFitnessValue(int* aBitString)
{
	double sum = 0;

	for(int i = 0; i < mN; ++i)
	{
		sum += aBitString[i];
	}

	if(sum < 3*mN/4 || sum >= 3*mN/4+mGapSize)
	{
		return  sum + mGapSize;
	}

	return 3*mN/4+mGapSize-sum;
}

double Jump::GetOffsetSpikeFitnessValue(int* aBitString)
{
	double sum = 0;

	for(int i = 0; i < mN; ++i)
	{
		sum += aBitString[i];
	}

	if(sum < 3 * mN / 4 || sum >= 3 * mN / 4 + mGapSize)
	{
		return sum + mGapSize;
	}

	if(sum == 3 * mN / 4 + mGapSize / 2)
	{
		return mN + mGapSize + 1;
	}

	return 3 * mN / 4 + mGapSize - sum;
}

void Jump::SetJumpType(JumpType aType)
{
	CheckGapSize();
	mType = aType;
}

JumpType Jump::GetJumpType()
{
	return mType;
}

void Jump::CheckGapSize()
{
	if((mType == JumpType::Offset || mType == JumpType::OffsetSpike) && mGapSize >= mN / 4)
	{
		std::cout << "The gap size " << mGapSize << " is to big for bitstring length " << mN << " when using the Offset or OffsetSpike type Jump!";
		exit(-1);
	}
}