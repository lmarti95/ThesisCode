#pragma once

#include <atomic>

#include "Benchmark.h"
#include "CostFunction.h"
#include "EvolutionaryAlgorithm.h"
#include "Jump.h"

class Menu
{
public:
	Menu();
	~Menu();

	void ContinousRun();

	void PrintAlgorithmChoices();
	void PrintCostFunctionChoices();
	void PrintJumpTypes();
	void PrintOpenWindow();
	void PrintShowWeights();
	void PrintForceShutdown();

	bool SetupAlgorithm(int aSelected);
	void SetSelectedAlgorithm(int aSelected);
	bool SetSelectedCostFunction(int aSelected);

	void IntegerInputValidation(int* aInput, int aFrom = INT_MIN, int aTo = INT_MAX, bool aExit = false);

	void OpenWindow(EvolutionaryAlgorithm* aEA);

	bool MSTCheck(int aNodes, int aEdges);

	static std::mutex mPrintMutex;

private:
	Benchmark mBenchmark;
	EvolutionaryAlgorithm* mEA = nullptr;
	CostFunction* mCostFunction = nullptr;
	int mAditionalSelection;
	int mN;

	JumpType mJumpType;
	int mJumpSize;

	std::vector<EvolutionaryAlgorithm*> mEAs;

	std::vector<std::thread*> mThreads;
#if GRAPHICS
	std::atomic<bool> mOpenGLRunning = false;
	bool mWeightVisibility = false;
#endif
};