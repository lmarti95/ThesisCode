#include "cGA.h"

#include <chrono>

cGA::cGA(int N, CostFunction* aCostFunction, int aMu) : GeneticAlgorithm(N, aCostFunction)
{
	mMu = aMu;
	mF = new double[mN];

#ifdef GRAPHICS
	mBitString = new int[mN];
#endif
}

cGA::~cGA()
{
	delete[] mF;

#ifdef GRAPHICS
	delete[] mBitString;
#endif
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
	mIterations = 0;
	double maximumFitnessValue = mCostFunction->GetMaximumFitnessValue();
	mFitnessValue = 0;
	int* X1;
	int* X2;
	double fitnessValueX1;
	double fitnessValueX2;
	int* Y1;
	int* Y2;

	SetF();

	auto start = std::chrono::steady_clock::now();

	while(mFitnessValue != maximumFitnessValue)
	{
		mIterations++;
		X1 = Sample();
		X2 = Sample();

		fitnessValueX1 = mCostFunction->GetFitnessValue(X1);
		fitnessValueX2 = mCostFunction->GetFitnessValue(X2);

		if(fitnessValueX1 >= fitnessValueX2)
		{
			Y1 = X1;
			Y2 = X2;
			mFitnessValue = fitnessValueX1;
		}
		else
		{
			Y1 = X2;
			Y2 = X1;
			mFitnessValue = fitnessValueX2;
		}

		#ifdef GRAPHICS
		{
			std::lock_guard<std::mutex> lg{mBitStringMutex};
			std::copy(Y1, Y1 + mN, mBitString);
		}
		#endif

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

		#ifdef GRAPHICS
			std::this_thread::sleep_for(std::chrono::milliseconds(mDelay));
		#endif
	}

	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsedSeconds = end - start;

	long long iterations = mIterations;

	return std::make_pair(iterations, elapsedSeconds.count());
}

std::vector<int>* cGA::GetBitString()
{
	std::lock_guard<std::mutex> lg{mBitStringMutex};
	std::vector<int>* bitString = new std::vector<int>;

	for(int i = 0; i < mN; ++i)
	{
		bitString->push_back(mBitString[i]);
	}

	return bitString;
}