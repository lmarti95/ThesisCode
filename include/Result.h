#pragma once

#include <string>

class Result {
public:
	Result(double aIterations, double aTime, int aRepeat, std::string aCostFunctionName, std::string aEAName, double aN, int aGap = 0, std::string aJumpType = "");

	std::string GetResult();
private:
	double mIterations;
	double mTime;
	int mRepeat;
	
	std::string mCostFunctionName;
	std::string mEAName;
	double mN;
	int mGap;
	std::string mJumpType;
};