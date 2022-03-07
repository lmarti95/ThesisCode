#include "Benchmark.h"
#include "cGA.h"
#include "Cliff.h"
#include "CostFunction.h"
#include "HybridGA.h"
#include "Jump.h"
#include "MST.h"
#include "MuPlusOneGA.h"
#include "SASD_OnePlusLambda.h"
#include "SD_OnePlusOne.h"
#include "SD_RLS.h"
#include "SD_RLS_m.h"
#include "SD_RLS_r.h"
#include "SD_RLS_STAR.h"

#include <iostream>

int main()
{
	std::cout << "Maximum number of threads used: " << std::thread::hardware_concurrency() << std::endl;

	std::vector<CostFunction*> toDeleteCostFunctions;
	std::vector<EvolutionaryAlgorithm*> toDeleteEvolutionaryAlgorithms;

	std::vector<int> testNs;
	testNs.push_back(10);
	testNs.push_back(12);
	testNs.push_back(15);
	testNs.push_back(18);
	testNs.push_back(21);
	testNs.push_back(25);
	testNs.push_back(30);
	testNs.push_back(40);
	testNs.push_back(50);

	Benchmark b;

	b.SetRepeat(100);

	//MST j(N, "7_21.mst");

	for(auto& N : testNs)
	{
		for(int gapSize = 2; gapSize < 7; ++gapSize)
		{
			if(N / 3 < gapSize)
			{
				break;
			}

			Jump* j = new Jump(N, gapSize, JumpType::Original);
			toDeleteCostFunctions.push_back(j);

			cGA* sd = new cGA(N, j, 4);

			HybridGA* sd2 = new HybridGA(N, j);

			MuPlusOneGA* sd3 = new MuPlusOneGA(N, j, 4);

			SASD_OnePlusLambda* sd4 = new SASD_OnePlusLambda(N, j, 4);

			SD_RLS* sd5 = new SD_RLS(N, j);

			SD_RLS_m* sd6 = new SD_RLS_m(N, j);

			SD_RLS_r* sd7 = new SD_RLS_r(N, j);

			SD_RLS_STAR* sd8 = new SD_RLS_STAR(N, j);

			SD_OnePlusOne* sd9 = new SD_OnePlusOne(N, j);

			toDeleteEvolutionaryAlgorithms.push_back(sd);
			toDeleteEvolutionaryAlgorithms.push_back(sd2);
			toDeleteEvolutionaryAlgorithms.push_back(sd3);
			toDeleteEvolutionaryAlgorithms.push_back(sd4);
			toDeleteEvolutionaryAlgorithms.push_back(sd5);
			toDeleteEvolutionaryAlgorithms.push_back(sd6);
			toDeleteEvolutionaryAlgorithms.push_back(sd7);
			toDeleteEvolutionaryAlgorithms.push_back(sd8);
			toDeleteEvolutionaryAlgorithms.push_back(sd9);

			b.ScheduleEA(sd);
			b.ScheduleEA(sd2);
			b.ScheduleEA(sd3);
			b.ScheduleEA(sd4);
			b.ScheduleEA(sd5);
			b.ScheduleEA(sd6);
			b.ScheduleEA(sd7);
			b.ScheduleEA(sd8);
			b.ScheduleEA(sd9);

		}
	}

	b.SaveResults("JumpOriginal.txt");

	for(auto& c : toDeleteCostFunctions)
	{
		delete c;
	}

	for(auto& ea : toDeleteEvolutionaryAlgorithms)
	{
		delete ea;
	}
}
