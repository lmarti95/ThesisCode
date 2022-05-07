#include "Benchmark.h"
#include "cGA.h"
#include "Cliff.h"
#include "CostFunction.h"
#include "HybridGA.h"
#include "Jump.h"
#include "Menu.h"
#include "MST.h"
#include "MSTVisualization.h"
#include "MuPlusOneGA.h"
#include "SASD_OnePlusLambda.h"
#include "SD_OnePlusOne.h"
#include "SD_RLS.h"
#include "SD_RLS_m.h"
#include "SD_RLS_STAR.h"

#if GRAPHICS
	#include "OpenGL.h"
#endif

#include <fstream>
#include <iostream>

bool CheckIfExists(EvolutionaryAlgorithm* aEA)
{
	std::string str = aEA->GetEAName() + "_" + aEA->GetCostFunctionName() + "_" + std::to_string((int)aEA->GetN()) + ".result";

	std::ifstream f(str.c_str());
	return f.good();
}

int main()
{
	Menu m;

	/*std::vector<EvolutionaryAlgorithm*> toDeleteEvolutionaryAlgorithms;

	std::vector<int> testNs;
	testNs.push_back(12);
	testNs.push_back(15);
	testNs.push_back(20);
	testNs.push_back(25);
	testNs.push_back(50);
	testNs.push_back(100);
	testNs.push_back(200);
	testNs.push_back(300);
	testNs.push_back(500);
	testNs.push_back(1000);

	Benchmark b;

	b.SetRepeat(100);

	for(auto& N : testNs)
	{
		for(int gapSize = 2; gapSize < 5; ++gapSize)
		{
			if(N / 3 < gapSize)
			{
				break;
			}

			Jump j(N, gapSize, JumpType::Original);

			cGA* sd1 = new cGA(N, &j, 4);

			HybridGA* sd2 = new HybridGA(N, &j);

			MuPlusOneGA* sd3 = new MuPlusOneGA(N, &j, 4);

			SASD_OnePlusLambda* sd4 = new SASD_OnePlusLambda(N, &j, 4);

			SD_RLS* sd5 = new SD_RLS(N, &j);

			SD_RLS_m* sd6 = new SD_RLS_m(N, &j);

			SD_RLS_STAR* sd7 = new SD_RLS_STAR(N, &j);

			SD_OnePlusOne* sd8 = new SD_OnePlusOne(N, &j);

			toDeleteEvolutionaryAlgorithms.push_back(sd1);
			toDeleteEvolutionaryAlgorithms.push_back(sd2);
			toDeleteEvolutionaryAlgorithms.push_back(sd3);
			toDeleteEvolutionaryAlgorithms.push_back(sd4);
			toDeleteEvolutionaryAlgorithms.push_back(sd5);
			toDeleteEvolutionaryAlgorithms.push_back(sd6);
			toDeleteEvolutionaryAlgorithms.push_back(sd7);
			toDeleteEvolutionaryAlgorithms.push_back(sd8);

			if(!CheckIfExists(sd1))
			{
				b.ScheduleEA(sd1);
			}

			if(!CheckIfExists(sd2))
			{
				b.ScheduleEA(sd2);
			}

			if(!CheckIfExists(sd3))
			{
				b.ScheduleEA(sd3);
			}
			if(!CheckIfExists(sd4))
			{
				b.ScheduleEA(sd4);
			}
			if(!CheckIfExists(sd5))
			{
				b.ScheduleEA(sd5);
			}
			if(!CheckIfExists(sd6))
			{
				b.ScheduleEA(sd6);
			}
			if(!CheckIfExists(sd7))
			{
				b.ScheduleEA(sd7);
			}
			if(!CheckIfExists(sd8))
			{
				b.ScheduleEA(sd8);
			}
		}
	}

	b.FinishThreads();

	for(auto& ea : toDeleteEvolutionaryAlgorithms)
	{
		delete ea;
	}*/
}
