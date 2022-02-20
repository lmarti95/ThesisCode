#include "cGA.h"

#include <chrono>

cGA::cGA(int N, CostFunction* aCostFunction, int aMu) : GeneticAlgorithm(N, aCostFunction)
{
	mMu = aMu;
	mF = new double[mN];
}

cGA::~cGA()
{
	delete[] mF;
}

void cGA::SetF()
{
	for(int i = 0; i < mN; ++i)
	{
		mF[i] = 0.5;
	}
}

int* cGA::Sample()
{
	int* sample = new int[mN];
	std::uniform_real_distribution<> p(0.0, 1.0);
	
	for(int i = 0; i < mN; ++i)
	{
		if(p(mRng) > 1-mF[i])
		{
			sample[i] = 1;
		}
		else
		{
			sample[i] = 0;
		}
	}

	return sample;
}

std::pair<long long, double> cGA::RunEA()
{
	long long iterations = 0;
	double maximumFitnessValue = mCostFunction->GetMaximumFitnessValue();
	double fitnessValue = 0;
	int* X1;
	int* X2;
	double fitnessValueX1;
	double fitnessValueX2;
	int* Y1;
	int* Y2;

	SetF();

	auto start = std::chrono::steady_clock::now();

	while(fitnessValue != maximumFitnessValue)
	{
		iterations++;
		X1 = Sample();
		X2 = Sample();

		fitnessValueX1 = mCostFunction->GetFitnessValue(X1);
		fitnessValueX2 = mCostFunction->GetFitnessValue(X2);

		if(fitnessValueX1 >= fitnessValueX2)
		{
			Y1 = X1;
			Y2 = X2;
			fitnessValue = fitnessValueX1;
		}
		else
		{
			Y1 = X2;
			Y2 = X1;
			fitnessValue = fitnessValueX2;
		}

		for(int i = 0; i < mN; ++i)
		{
			mF[i] = mF[i] + 1 / (double)mMu * (Y1[i] - Y2[i]);
		}

		for(int i = 0; i < mN; ++i)
		{
			mF[i] = std::max(1 / (double)mN, std::min(mF[i], 1 - 1 / (double)mN));
		}

		delete[] X1;
		delete[] X2;
	}

	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsedSeconds = end - start;

	return std::make_pair(iterations, elapsedSeconds.count());
}