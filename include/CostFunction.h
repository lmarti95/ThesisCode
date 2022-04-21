#pragma once

#include <string>

class CostFunction
{
public:
	CostFunction(double aN) { mN = aN; }

	CostFunction(const CostFunction& aOld);

	virtual double GetMaximumFitnessValue() = 0;
	virtual double GetFitnessValue(int* aBitString) = 0;
	virtual double GetFitnessValue(int aChange) = 0;
	virtual void ApplyChange(int aChange) = 0;
	virtual std::string GetCostFunctionName() = 0;

	virtual double GetSum() = 0;
	virtual void CalculateSum(int* aBitString) = 0;

	virtual double FitnessValueToSum(double aFitness) = 0;

	void SetN(double aN) { mN = aN; }
	double GetN() { return mN; }
protected:
	double mN;
};

