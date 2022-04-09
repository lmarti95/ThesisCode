#include "HybridGA.h"

#include "Cliff.h"
#include "CostFunction.h"
#include "Jump.h"

#include <algorithm>
#include <chrono>

HybridGA::HybridGA(int aN, CostFunction* aCostFunction) : GeneticAlgorithm(aN, aCostFunction)
{
	SetUpPermutation();

#if GRAPHICS
	mBitString = new int[mN];
#endif
}

void HybridGA::SetUpPermutation()
{
	for(int i = 0; i < mN; ++i)
	{
		mPermutation.push_back(i);
	}

#if GRAPHICS
	delete[] mBitString;
#endif
}

int* HybridGA::MajorityVoting(std::vector<int*> aParents)
{
	int* offspring = new int[mN];
	
	for(int i = 0; i < mN; ++i)
	{
		unsigned int ones = 0;
		for(unsigned int j = 0; j < aParents.size(); ++j)
		{
			if(aParents.at(j)[i] == 1)
			{
				ones++;
			}
		}

		if(ones > aParents.size() / 2)
		{
			offspring[i] = 1;
		}
		else
		{
			offspring[i] = 0;
		}
	}

	return offspring;
}

std::pair<long long, double> HybridGA::RunEA()
{
	mIterations = 0;
	mFitnessValue = 0;
	double maximumFitnessValue = mCostFunction->GetMaximumFitnessValue();
	std::vector<int*> parents;

	auto start = std::chrono::steady_clock::now();

	while(maximumFitnessValue != mFitnessValue)
	{
		for(int i = 0; i < 3; ++i)
		{
			int* bitString = CreateRandomBitString();
			mCostFunction->CalculateSum(bitString);
			mFitnessValue = mCostFunction->GetFitnessValue(0);
			bool updated = true;

			while(updated)
			{
				updated = false;
				std::shuffle(mPermutation.begin(), mPermutation.end(), mRng);

				for(auto& position : mPermutation)
				{
					mIterations++;
					int change = FlipBitBasedOnPosition(bitString, position);

					double newFitnessValue;

					if(mFitnessChangePossible)
					{
						newFitnessValue = mCostFunction->GetFitnessValue(change);
					}
					else
					{
						newFitnessValue = mCostFunction->GetFitnessValue(bitString);
					}

					if(newFitnessValue > mFitnessValue)
					{
						mCostFunction->ApplyChange(change);
						mFitnessValue = newFitnessValue;
						updated = true;
					}
					else
					{
						FlipBitBasedOnPosition(bitString, position);
					}
				}
			}

			parents.push_back(bitString);
		}

		int* offspring = MajorityVoting(parents);

		#if GRAPHICS
		{
			std::lock_guard<std::mutex> lg{mBitStringMutex};
			std::copy(offspring, offspring + mN, mBitString);
		}
		#endif

		mIterations++;

		mFitnessValue = mCostFunction->GetFitnessValue(offspring);
		
		delete[] offspring;
		for(auto& p : parents)
		{
			delete[] p;
		}
		parents.clear();

		#if GRAPHICS
			std::this_thread::sleep_for(std::chrono::milliseconds(mDelay));
		#endif
	}

	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsedSeconds = end - start;

	long long iterations = mIterations;

	return std::make_pair(iterations, elapsedSeconds.count());
}

std::vector<int>* HybridGA::GetBitString()
{
#if GRAPHICS
	std::lock_guard<std::mutex> lg{mBitStringMutex};
	std::vector<int>* bitString = new std::vector<int>;

	for(int i = 0; i < mN; ++i)
	{
		bitString->push_back(mBitString[i]);
	}

	return bitString;
#else
	return nullptr;
#endif
}