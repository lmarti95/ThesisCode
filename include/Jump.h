#pragma once

#include "CostFunction.h"

enum class JumpType{ Original, Offset, OffsetSpike };

class Jump : public CostFunction
{
public:
	Jump(int aN, int aGapSize, JumpType aType = JumpType::Original);

	double GetMaximumFitnessValue() override;
	double GetFitnessValue(int* aBitString) override;
	double GetFitnessValue(int aChange) override;

	double GetOriginalFitnessValue(int* aBitString);
	double GetOffsetFitnessValue(int* aBitString);
	double GetOffsetSpikeFitnessValue(int* aBitString);
	double GetOriginalFitnessValue(int aChange);
	double GetOffsetFitnessValue(int aChange);
	double GetOffsetSpikeFitnessValue(int aChange);

	void ApplyChange(int aChange) override { mSum += aChange; }
	
	std::string GetCostFunctionName() override { return "Jump(" +std::to_string(mGapSize) + ")" ; }

	void SetJumpType(JumpType aType);
	JumpType GetJumpType();
	std::string GetJumpTypeString();

	int GetGapSize() { return mGapSize; }
	void CheckGapSize();

	void SetSum(double aSum) { mSum = aSum; }
private:
	int mGapSize;
	double mSum = 0;
	JumpType mType;
};
