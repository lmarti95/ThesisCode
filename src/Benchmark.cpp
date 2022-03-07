#include "Benchmark.h"

#include "Jump.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>
#include <time.h>

Benchmark::~Benchmark()
{
	for(auto& res : mResults)
	{
		delete res;
	}
}

void Benchmark::Shutdown()
{
	while(mPlanToRun > mFinished)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	for(auto& t : mThreads)
	{
		t->join();
	}
}

void Benchmark::RunEA(EvolutionaryAlgorithm* aEA)
{
	long long iterations = 0;
	double time = 0;

	for(int i = 0; i < mRepeat; ++i)
	{
		auto result = aEA->RunEA();
		iterations += result.first;
		time += result.second;
	}

	double averageIteration = iterations / mRepeat;
	double averageTime = time / (double)mRepeat;

	std::cout << averageIteration << " average iterations in " << averageTime << "s on average with " << aEA->GetEAName() << " on " << aEA->GetCostFunctionName() << ", ran it " << std::to_string(mRepeat) << " times on N: " << aEA->GetN() << std::endl;

	Result* res;
	
	if(dynamic_cast<Jump*>(aEA->GetCostFunction()) == nullptr)
	{
		res = new Result(averageIteration, averageTime, mRepeat, aEA->GetCostFunctionName(), aEA->GetEAName(), aEA->GetN());
	}
	else
	{
		res = new Result(averageIteration, averageTime, mRepeat, aEA->GetCostFunctionName(), aEA->GetEAName(), aEA->GetN(), dynamic_cast<Jump*>(aEA->GetCostFunction())->GetGapSize(), dynamic_cast<Jump*>(aEA->GetCostFunction())->GetJumpTypeString());
	}

	mFinished++;

	mResultsMutex.lock();
	mResults.push_back(res);
	mResultsMutex.unlock();

	mActiveThreadsMutex.lock();
	mActiveThreads--;
	mActiveThreadsMutex.unlock();
}

void Benchmark::ScheduleEA(EvolutionaryAlgorithm* aEA)
{
	mPlanToRun++;
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
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
		
	}
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

void Benchmark::SaveResults()
{
	while(mPlanToRun > mFinished)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	/*std::time_t now = time(0);
	char str[26];
	ctime_s(str, sizeof str, &now);*/

	std::ofstream file;
	//file.open(std::string(str) + ".txt");
	file.open("output.txt");
	for(auto& res : mResults)
	{
		file << res->GetResult();
		file << std::endl;
	}

	file.close();
}
