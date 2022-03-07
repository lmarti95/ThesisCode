#include "Result.h"

#include <iomanip>
#include <sstream>

Result::Result(double aIterations, double aTime, int aRepeat, std::string aCostFunctionName, std::string aEAName, int aN, int aGap, std::string aJumpType)
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
	std::stringstream streamIterations;
	streamIterations << std::fixed << std::setprecision(0) << mIterations;
	std::string iterationsString = streamIterations.str();

	std::stringstream streamTime;
	streamTime << std::fixed << std::setprecision(4) << mTime;
	std::string timeString = streamTime.str();


	if(mJumpType.compare("") == 0)
	{
		std::string res = iterationsString + " " + timeString + " " + std::to_string(mRepeat) + " " + mCostFunctionName + " " + mEAName + " " + std::to_string(mN);
		return res;
	}
	else
	{
		std::string res = iterationsString + " " + timeString + " " + std::to_string(mRepeat) + " " + mCostFunctionName + " " + mEAName + " " + std::to_string(mN) + " " + std::to_string(mGap) + " " + mJumpType;
		return res;
	}
}
