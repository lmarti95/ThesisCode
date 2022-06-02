#include "Benchmark.h"

#include "Jump.h"
#include "Menu.h"

#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>
#include <time.h>

Benchmark::~Benchmark()
{
}

void Benchmark::FinishThreads()
{
	while(mPlanToRun > mFinished)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	for(auto& t : mThreads)
	{
		t->join();
		delete t;
	}

	mPlanToRun = 0;
	mFinished = 0;
}

void Benchmark::RunEA(EvolutionaryAlgorithm* aEA)
{
	int repeat = mRepeat;
	bool gotThread = false;
	while(!gotThread)
	{
		mActiveThreadsMutex.lock();
		if(mActiveThreads < mMaximumThreads)
		{
			mActiveThreads++;
			gotThread = true;;
			mActiveThreadsMutex.unlock();
		}
		else
		{
			mActiveThreadsMutex.unlock();
			std::this_thread::sleep_for(std::chrono::seconds(2));
		}
	}
	Menu::mPrintMutex.lock();
	std::cout << aEA->GetEAName() << " on " << aEA->GetCostFunctionName() << " is now running on N: " << aEA->GetN() << std::endl;
	Menu::mPrintMutex.unlock();

	long long iterations = 0;
	double time = 0;

	std::vector<long> iterationsVector;

	for(int i = 0; i < repeat; ++i)
	{
		auto result = aEA->RunEA();
		iterationsVector.push_back(result.first);
		iterations += result.first;
		time += result.second;
	}

	std::string filename = aEA->GetEAName() + "_" + aEA->GetCostFunctionName() + "_" + std::to_string((int)aEA->GetN()) + ".result";

	SaveEachIteration(iterationsVector, filename);

	double averageIteration = iterations / (double)repeat;
	double averageTime = time / (double)repeat;

	std::stringstream streamIterations;
	streamIterations << std::fixed << std::setprecision(0) << averageIteration;
	std::string iterationsString = streamIterations.str();
	Menu::mPrintMutex.lock();
	std::cout << iterationsString << " average iterations in " << averageTime << "s on average with " << aEA->GetEAName() << " on " << aEA->GetCostFunctionName() << ", ran it " << std::to_string(repeat) << " times on N: " << aEA->GetN() << std::endl;
	Menu::mPrintMutex.unlock();

	mActiveThreadsMutex.lock();
	mFinished++;
	mActiveThreads--;
	mActiveThreadsMutex.unlock();
}

void Benchmark::ScheduleEA(EvolutionaryAlgorithm* aEA)
{
	if(mActiveThreads >= mMaximumThreads)
	{
		std::cout << aEA->GetEAName() << " on " << aEA->GetCostFunctionName() << " is scheduled to run on N: " << aEA->GetN() << ", but all threads are active now." << std::endl;
	}
	mPlanToRun++;
	std::thread* t = new std::thread(&Benchmark::RunEA, this, aEA);
	mThreads.push_back(t);
}

void Benchmark::SavePlot(std::vector<int> aX, std::vector<double> aY, std::string aXLabel, std::string aYLabel, std::string aTitle)
{
	/*if(!std::filesystem::is_directory("Output") || !std::filesystem::exists("Output"))
	{
		std::filesystem::create_directory("Output");
	}*/

	std::ofstream file;
	file.open("Output/" + aTitle + ".txt");

	file << aXLabel << " = [";

	for(auto& x : aX)
	{
		if(&x == &aX.back())
		{
			file << x << "]" << std::endl;
		}
		else
		{
			file << x << ", ";
		}
	}

	file << aYLabel << " = [";

	for(auto& y : aY)
	{
		if(&y == &aY.back())
		{
			file << y << "]" << std::endl << std::endl;
		}
		else
		{
			file << y << ", ";
		}
	}

	file << "plt.plot(" << aXLabel << "," << aYLabel << ")" << std::endl;

	file << "plt.xlabel('" << aXLabel << "')" << std::endl;
	file << "plt.ylabel('" << aYLabel << "')" << std::endl;
	file << "plt.title('" << aTitle << "')" << std::endl;

	file.close();

	std::cout << aTitle << ".txt was created" << std::endl;
}

void Benchmark::SaveEachIteration(std::vector<long> aIterations, std::string aFilename)
{
	std::ofstream file;
	file.open(aFilename);
	for(auto& it : aIterations)
	{
		file << it << std::endl;
	}

	file.close();
}
