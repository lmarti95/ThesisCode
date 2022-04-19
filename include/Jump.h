#pragma once

#include "CostFunction.h"

#include <atomic>

enum class JumpType{ Original, Offset, OffsetSpike };

class Jump : public CostFunction
{
public:
	Jump(int aN, int aGapSize, JumpType aType = JumpType::Original);

	Jump(const Jump& aOld);

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

	void CalculateSum(int* aBitString) override;
	
	std::string GetCostFunctionName() override { return "Jump(" +std::to_string(mGapSize) + ") " + JumpTypeToString(); }

	void SetJumpType(JumpType aType);
	JumpType GetJumpType();
	std::string GetJumpTypeString();

	int GetGapSize() { return mGapSize; }
	void CheckGapSize();

	void SetSum(double aSum) { mSum = aSum; }
	double GetSum() override { return mSum; }

	double FitnessValueToSum(double aFitness) override;

	std::string JumpTypeToString();
private:
	int mGapSize;

#if GRAPHICS
	std::atomic_long mSum = {0};
#else
	double mSum = 0;
#endif	

	JumpType mType;
};
