#pragma once

#include "EvolutionaryAlgorithm.h"
#include "Result.h"

#include <atomic>
#include <mutex>
#include <thread>

class Benchmark {
public:
	~Benchmark();
	void FinishThreads();
	int GetRepeat() { return mRepeat; }
	void SetRepeat(int aRepeat) { mRepeat = aRepeat; }

	int GetActiveThreads() { std::lock_guard<std::mutex> lg{mActiveThreadsMutex}; return mActiveThreads; }

	int GetMaximumThreads() { return mMaximumThreads; }
	void SetMaximumThreads(int aMaximumThreads) { mMaximumThreads = aMaximumThreads; }

	void ScheduleEA( EvolutionaryAlgorithm* aEA);

	void SavePlot(std::vector<int> aX, std::vector<double> aY, std::string aXLabel, std::string aYLabel, std::string aTitle);

	void SaveResults(std::string aFilename);

	void SaveEachIteration(std::vector<long> aIterations, std::string aFilename);
private:
	void RunEA(EvolutionaryAlgorithm* aEA);

	int mRepeat = 1000;

	std::mutex mActiveThreadsMutex;
	int mActiveThreads = 0;
	int mMaximumThreads = std::thread::hardware_concurrency();

	std::vector<std::thread*> mThreads;

	std::mutex mResultsMutex;
	std::vector<Result*> mResults;

	std::atomic_int  mPlanToRun = {0};
	std::atomic_int mFinished = {0};
};
