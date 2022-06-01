#include "Jump.h"

#include <iostream>

Jump::Jump(int aN, int aGapSize, JumpType aType) : CostFunction(aN)
{
	mGapSize = aGapSize;
	mType = aType;

	CheckGapSize();
}

Jump::Jump(const Jump& aOld) : CostFunction(aOld)
{
	mSum = 0;
	mGapSize = aOld.mGapSize;
	mType = aOld.mType;
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

double Jump::GetFitnessValue(int aChange)
{
	switch(mType)
	{
	case JumpType::Original:
		return GetOriginalFitnessValue(aChange);
	case JumpType::Offset:
		return GetOffsetFitnessValue(aChange);
	case JumpType::OffsetSpike:
		return GetOffsetSpikeFitnessValue(aChange);
	default:
		return GetOriginalFitnessValue(aChange);
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

double Jump::GetOriginalFitnessValue(int aChange)
{
	double sum = mSum + aChange;

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

	if(sum <= mN/4*3 || sum >= mN/4*3+mGapSize)
	{
		return  sum + mGapSize;
	}

	return mN/4*3+mGapSize-sum;
}

double Jump::GetOffsetFitnessValue(int aChange)
{
	double sum = mSum + aChange;

	if(sum <=  mN / 4*3 || sum >=  mN / 4*3 + mGapSize)
	{
		return  sum + mGapSize;
	}

	return  mN / 4*3 + mGapSize - sum;
}

double Jump::GetOffsetSpikeFitnessValue(int* aBitString)
{
	double sum = 0;

	for(int i = 0; i < mN; ++i)
	{
		sum += aBitString[i];
	}

	if(sum <=  mN / 4*3 || sum >=  mN / 4*3 + mGapSize)
	{
		return sum + mGapSize;
	}

	if(sum ==  mN / 4*3 + mGapSize / 2)
	{
		return mN + mGapSize + 1;
	}

	return  mN / 4*3 + mGapSize - sum;
}

double Jump::GetOffsetSpikeFitnessValue(int aChange)
{
	double sum = mSum + aChange;

	if(sum <=  mN / 4*3 || sum >=  mN / 4*3 + mGapSize)
	{
		return sum + mGapSize;
	}

	if(sum ==  mN / 4*3 + mGapSize / 2)
	{
		return mN + mGapSize + 1;
	}

	return  mN / 4*3 + mGapSize - sum;
}

void Jump::CalculateSum(int* aBitString)
{
	double sum = 0;

	for(int i = 0; i < mN; ++i)
	{
		sum += aBitString[i];
	}

	mSum = sum;
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

std::string Jump::GetJumpTypeString()
{
	switch(mType)
	{
	case JumpType::Original:
		return "Original";
	case JumpType::Offset:
		return "Offset";
	case JumpType::OffsetSpike:
		return "OffsetSpike";
	default:
		return "Original";
	}
}

void Jump::CheckGapSize()
{
	if(mGapSize > mN)
	{
		std::cout << "The gap size " << mGapSize << " is to big for bitstring length " << mN << std::endl;
		exit(-1);
	}

	if((mType == JumpType::Offset || mType == JumpType::OffsetSpike) && mGapSize > mN / 4)
	{
		std::cout << "The gap size " << mGapSize << " is to big for bitstring length " << mN << " when using the Offset or OffsetSpike type Jump!";
		exit(-1);
	}
}

double Jump::FitnessValueToSum(double aFitness)
{
	switch(mType)
	{
	case JumpType::Original:
		if(aFitness > mGapSize)
		{
			return  aFitness- mGapSize;
		}

		return mN - aFitness;
		break;
	case JumpType::Offset:
		if(aFitness > mGapSize)
		{
			return  aFitness - mGapSize;
		}

		return mN/4*3+mGapSize - aFitness;
		break;
	case JumpType::OffsetSpike:
		if(aFitness > mGapSize && aFitness != GetMaximumFitnessValue())
		{
			return  aFitness - mGapSize;
		}

		if(aFitness == GetMaximumFitnessValue())
		{
			return  mN / 4*3 + mGapSize / 2;
		}

		return mN - (mN / 4 - mGapSize) - aFitness;
		break;
	default:
		return -1;
		break;
	}
}

std::string Jump::JumpTypeToString()
{
	switch(mType)
	{
	case JumpType::Original:
		return "Original";
		break;
	case JumpType::Offset:
		return "Offset";
		break;
	case JumpType::OffsetSpike:
		return "Offset Spike";
		break;
	default:
		return "Non existent Jump Type";
		break;
	}
}
