#include "Menu.h"

#include "cGA.h"
#include "Cliff.h"
#include "CostFunction.h"
#include "HybridGA.h"
#include "Jump.h"
#include "MST.h"
#include "MSTGenerator.h"
#include "MSTVisualization.h"
#include "MuPlusOneGA.h"
#include "OpenGL.h"
#include "SASD_OnePlusLambda.h"
#include "SD_OnePlusOne.h"
#include "SD_RLS.h"
#include "SD_RLS_m.h"
#include "SD_RLS_STAR.h"

#include <iostream>
#include <string>

std::mutex Menu::mPrintMutex;

Menu::Menu()
{
#if GRAPHICS
	std::cout << "This version of the program was compiled for visual representation" << std::endl;
#else
	std::cout << "This version of the program was compiled for performance" << std::endl;
#endif
	ContinousRun();
}

Menu::~Menu()
{
	mBenchmark.FinishThreads();

	for(auto& t : mThreads)
	{
		t->join();
	}

	for(auto& ea : mEAs)
	{
		delete ea;
	}

	if(mCostFunction != nullptr)
	{
		delete mCostFunction;
	}
}

void Menu::ContinousRun()
{
	int selected;
	while(true)
	{
		PrintAlgorithmChoices();
		IntegerInputValidation(&selected, 0, 7, true);

		if(selected == 9)
		{
			if(mBenchmark.GetActiveThreads() > 0)
			{
				PrintForceShutdown();
				IntegerInputValidation(&selected, 0, 1);
				if(selected == 0)
				{
					continue;
				}
				else
				{
					exit(0);
				}
			}
			return;
		}

		if(SetupAlgorithm(selected))
		{
#if GRAPHICS
			if(!mOpenGLRunning)
			{
				PrintOpenWindow();
				int windowSelection;
				IntegerInputValidation(&windowSelection, 0, 1);
				if(windowSelection == 1)
				{
					mPrintMutex.lock();
					std::cout << std::endl;
					std::cout << "Specify Iteration Delay in ms" << std::endl;
					mPrintMutex.unlock();

					int delay;

					IntegerInputValidation(&delay, 0);

					mEA->SetDelay(delay);

					mBenchmark.SetRepeat(1);

					if(dynamic_cast<MST*>(mCostFunction) != nullptr)
					{
						PrintShowWeights();
						int weight;
						IntegerInputValidation(&weight, 0, 1);
						if(weight == 0)
						{
							mWeightVisibility = false;
						}
						else
						{
							mWeightVisibility = true;
						}
					}

					std::thread* t = new std::thread(&Menu::OpenWindow, this, mEA);
					mThreads.push_back(t);
				}
			}
#endif

			mBenchmark.ScheduleEA(mEA);
			mEAs.push_back(mEA);
		}
	}
}

void Menu::PrintAlgorithmChoices()
{
	std::lock_guard<std::mutex> lg{mPrintMutex};
	std::cout << std::endl;
	std::cout << "Select one of the following algorithms: " << std::endl;
	std::cout << "0 - compact GA" << std::endl;
	std::cout << "1 - Hybrid GA" << std::endl;
	std::cout << "2 - (Mu + 1) GA" << std::endl;
	std::cout << "3 - SASD-(1+Lambda)" << std::endl;
	std::cout << "4 - SD-(1+1)" << std::endl;
	std::cout << "5 - SD-RLS" << std::endl;
	std::cout << "6 - SD-RLS^m" << std::endl;
	std::cout << "7 - SD-RLS*" << std::endl;
	std::cout << "9 - Exit" << std::endl;
}

void Menu::PrintCostFunctionChoices()
{
	std::lock_guard<std::mutex> lg{mPrintMutex};
	std::cout << std::endl;
	std::cout << "Select one of the following cost functions: " << std::endl;
	std::cout << "0 - Cliff" << std::endl;
	std::cout << "1 - Jump" << std::endl;
	std::cout << "2 - MST" << std::endl;
	if(mCostFunction != nullptr && (dynamic_cast<Jump*>(mCostFunction) == nullptr ||
		((dynamic_cast<Jump*>(mCostFunction)->GetGapSize() < mN && dynamic_cast<Jump*>(mCostFunction)->GetJumpType() == JumpType::Original)
			|| dynamic_cast<Jump*>(mCostFunction)->GetGapSize() < mN / 4)))
	{
		std::cout << "3 - Use previously selected cost function" << std::endl;
	}

	std::cout << "9 - Back" << std::endl;
}

void Menu::PrintJumpTypes()
{
	std::lock_guard<std::mutex> lg{mPrintMutex};
	std::cout << std::endl;
	std::cout << "Select one of the following jump types: " << std::endl;
	std::cout << "0 - Original" << std::endl;
	std::cout << "1 - Offset" << std::endl;
	std::cout << "2 - Offset Spike" << std::endl;
	std::cout << "9 - Back" << std::endl;
}

void Menu::PrintOpenWindow()
{
	std::lock_guard<std::mutex> lg{mPrintMutex};
	std::cout << std::endl;
	std::cout << "Would you like to open a window to show the latest algorithm running?" << std::endl;
	std::cout << "0 - No" << std::endl;
	std::cout << "1 - Yes" << std::endl;
}

void Menu::PrintShowWeights()
{
	std::lock_guard<std::mutex> lg{mPrintMutex};
	std::cout << std::endl;
	std::cout << "Would you like to see the edges weigt?" << std::endl;
	std::cout << "0 - No" << std::endl;
	std::cout << "1 - Yes" << std::endl;
}

void Menu::PrintForceShutdown()
{
	std::lock_guard<std::mutex> lg{mPrintMutex};
	std::cout << std::endl;
	std::cout << "There are algorithms running, would you like to wait for them to finish or shutdown the program?" << std::endl;
	std::cout << "0 - Wait" << std::endl;
	std::cout << "1 - Shutdown" << std::endl;
}

bool Menu::SetupAlgorithm(int aSelected)
{
	if(aSelected == 0 || aSelected == 2 || aSelected == 3)
	{
		if(aSelected == 0 || aSelected == 3)
		{
			std::lock_guard<std::mutex> lg{mPrintMutex};
			std::cout << std::endl;
			std::cout << "Type in population size" << std::endl;
		}
		else
		{
			std::lock_guard<std::mutex> lg{mPrintMutex};
			std::cout << "Type in parent size" << std::endl;
		}
		
		IntegerInputValidation(&mAditionalSelection, 1);
	}
	
	mPrintMutex.lock();
	std::cout << std::endl;
	std::cout << "Type in bit string length or in case of MST the number of edges" << std::endl;
	mPrintMutex.unlock();
	IntegerInputValidation(&mN, 1);

	PrintCostFunctionChoices();
	int selectedCostFunction;
	int validationTo = 2;
	if(mCostFunction != nullptr && (dynamic_cast<Jump*>(mCostFunction) == nullptr ||
		((dynamic_cast<Jump*>(mCostFunction)->GetGapSize() < mN && dynamic_cast<Jump*>(mCostFunction)->GetJumpType() == JumpType::Original)
			|| dynamic_cast<Jump*>(mCostFunction)->GetGapSize() < mN/4)))
	{
		validationTo = 3;
	}
	IntegerInputValidation(&selectedCostFunction, 0, validationTo, true);
	if(!SetSelectedCostFunction(selectedCostFunction))
	{
		return false;
	}
	SetSelectedAlgorithm(aSelected);

	return true;
}

void Menu::SetSelectedAlgorithm(int aSelected)
{
	switch(aSelected)
	{
		case 0:
			mEA = new cGA(mN, mCostFunction, mAditionalSelection);
			break;
		case 1:
			mEA = new HybridGA(mN, mCostFunction);
			break;
		case 2:
			mEA = new MuPlusOneGA(mN, mCostFunction, mAditionalSelection);
			break;
		case 3:
			mEA = new SASD_OnePlusLambda(mN, mCostFunction, mAditionalSelection);
			break;
		case 4:
			mEA = new SD_OnePlusOne(mN, mCostFunction);
			break;
		case 5:
			mEA = new SD_RLS(mN, mCostFunction);
			break;
		case 6:
			mEA = new SD_RLS_m(mN, mCostFunction);
			break;
		case 7:
			mEA = new SD_RLS_STAR(mN, mCostFunction);
			break;
	}
}

bool Menu::SetSelectedCostFunction(int aSelected)
{
	if(aSelected == 3)
	{
		mCostFunction->SetN(mN);
		return true;
	}
	if(mCostFunction != nullptr)
	{
		delete mCostFunction;
		mCostFunction = nullptr;
	}

	if(aSelected == 0)
	{
		mCostFunction = new Cliff(mN);
		return true;
	}

	if(aSelected == 1)
	{
		PrintJumpTypes();
		int selectedJumpType;
		IntegerInputValidation(&selectedJumpType, 0, 2, true);

		if(selectedJumpType == 9)
		{
			return false;
		}

		switch(selectedJumpType)
		{
			case 0:
				mJumpType = JumpType::Original;
				break;
			case 1:
				mJumpType = JumpType::Offset;
				break;
			case 2:
				mJumpType = JumpType::OffsetSpike;
				break;
			default:
				mJumpType = JumpType::Original;
				break;
		}

		mPrintMutex.lock();
		std::cout << std::endl;
		std::cout << "Type in jump length" << std::endl;
		mPrintMutex.unlock();

		int validTo = mN;
		if(mJumpType != JumpType::Original)
		{
			validTo = mN/4;
		}
		IntegerInputValidation(&mJumpSize, 1, validTo);

		mCostFunction = new Jump(mN, mJumpSize, mJumpType);
		return true;
	}

	if(aSelected == 2)
	{
		bool check = false;
		int nodes;
		int edges;

		while(!check)
		{
			mPrintMutex.lock();
			std::cout << std::endl;
			std::cout << "Type in number of nodes" << std::endl;
			mPrintMutex.unlock();

			IntegerInputValidation(&nodes, 2);

			edges = mN;

			check = MSTCheck(nodes, edges);
		}

		MSTGenerator mst;
		mst.CreateMST(nodes, edges);

		mN = edges;

		std::string filename = std::to_string(nodes) + "_" + std::to_string(edges) + ".mst";

		mCostFunction = new MST(mN, filename);
		return true;
	}

	return false;
}

void Menu::IntegerInputValidation(int* aInput, int aFrom, int aTo, bool aExit)
{
	std::cin >> *aInput;
	while(!std::cin || !((*aInput >= aFrom && *aInput <= aTo) || (aExit && *aInput == 9)))
	{
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		mPrintMutex.lock();
		std::cout << std::endl;
		std::cout << "Invalid input!" << std::endl;
		mPrintMutex.unlock();

		std::cin >> *aInput;
	}
}

void Menu::OpenWindow(EvolutionaryAlgorithm* aEA)
{
	#if GRAPHICS
	mOpenGLRunning = true;
	OpenGL og;
	og.Setup(aEA);
	og.SetWeightVisibility(mWeightVisibility);
	og.Draw();
	og.ShutDown();
	mOpenGLRunning = false;
	#endif
}

bool Menu::MSTCheck(int aNodes, int aEdges)
{
	if(aEdges < aNodes - 1)
	{
		std::lock_guard<std::mutex> lg{mPrintMutex};
		std::cout << "Too many nodes specified!" << std::endl;
		return false;
	}

	if(aEdges > aNodes * (aNodes - 1) / 2)
	{
		std::lock_guard<std::mutex> lg{mPrintMutex};
		std::cout << "Too few nodes specified!" << std::endl;
		return false;
	}

	return true;
}
