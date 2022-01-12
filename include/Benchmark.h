#pragma once

#include "EvolutionaryAlgorithm.h"

class Benchmark {
public:
	int GetRepeat() { return mRepeat; }
	void SetRepeat(int aRepeat) { mRepeat = aRepeat; }

	std::pair<double, double> RunEA( EvolutionaryAlgorithm* aEA);

	void SavePlot(std::vector<int> aX, std::vector<double> aY, std::string aXLabel, std::string aYLabel, std::string aTitle);
private:
	int mRepeat = 100;
};
