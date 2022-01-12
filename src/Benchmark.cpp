#include "Benchmark.h"

#include <filesystem>
#include <fstream>
#include <iostream>

std::pair<double, double> Benchmark::RunEA(EvolutionaryAlgorithm* aEA)
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

	return std::make_pair(averageIteration, averageTime);	
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
