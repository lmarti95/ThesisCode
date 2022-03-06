#include "Result.h"

Result::Result(double aIterations, double aTime, int aRepeat, std::string aCostFunctionName, std::string aEAName, double aN, int aGap, std::string aJumpType)
{
	mIterations = aIterations;
	mTime = aTime;
	mRepeat = aRepeat;
	mCostFunctionName = aCostFunctionName;
	mEAName = aEAName;
	mN = aN;
	mGap = aGap;
	mJumpType = aJumpType;
}

std::string Result::GetResult()
{
	if(mJumpType.compare("") == 0)
	{
		std::string res = std::to_string(mIterations) + " " + std::to_string(mTime) + " " + std::to_string(mRepeat) + " " + mCostFunctionName + " " + mEAName + " " + std::to_string(mN);
		return res;
	}
	else
	{
		std::string res = std::to_string(mIterations) + " " + std::to_string(mTime) + " " + std::to_string(mRepeat) + " " + mCostFunctionName + " " + mEAName + " " + std::to_string(mN) + " " + std::to_string(mGap) + " " + mJumpType;
		return res;
	}
}
