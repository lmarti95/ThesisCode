#pragma once

#include "CostFunction.h"

enum class JumpType{ Original, Offset, OffsetSpike };

class Jump : public CostFunction
{
public:
	Jump(int aN, int aGapSize, JumpType aType = JumpType::Original);

	double GetMaximumFitnessValue() override;
	double GetFitnessValue(int* aBitString) override;

	double GetOriginalFitnessValue(int* aBitString);
	double GetOffsetFitnessValue(int* aBitString);
	double GetOffsetSpikeFitnessValue(int* aBitString);

	std::string GetCostFunctionName() override { return "Jump(" +std::to_string(mGapSize) + ")" ; }

	void SetJumpType(JumpType aType);
	JumpType GetJumpType();

	void CheckGapSize();
private:
	int mGapSize;
	JumpType mType;
};
