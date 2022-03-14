#include "Benchmark.h"
#include "cGA.h"
#include "Cliff.h"
#include "CostFunction.h"
#include "HybridGA.h"
#include "Jump.h"
#include "MST.h"
#include "MuPlusOneGA.h"
#include "OpenGL.h"
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

	std::vector<EvolutionaryAlgorithm*> toDeleteEvolutionaryAlgorithms;

	std::vector<int> testNs;
	testNs.push_back(100);
	testNs.push_back(200);
	testNs.push_back(300);
	testNs.push_back(400);
	testNs.push_back(500);
	testNs.push_back(600);
	testNs.push_back(700);
	testNs.push_back(800);
	testNs.push_back(900);
	testNs.push_back(1000);

	Benchmark b;

	b.SetRepeat(100);

	for(auto& N : testNs)
	{
		for(int gapSize = 2; gapSize < 2; ++gapSize)
		{
			if(N / 3 < gapSize)
			{
				break;
			}

			Jump j(N, gapSize, JumpType::Original);

			cGA* sd = new cGA(N, &j, 4);

			HybridGA* sd2 = new HybridGA(N, &j);

			MuPlusOneGA* sd3 = new MuPlusOneGA(N, &j, 4);

			SASD_OnePlusLambda* sd4 = new SASD_OnePlusLambda(N, &j, 4);

			SD_RLS* sd5 = new SD_RLS(N, &j);

			SD_RLS_m* sd6 = new SD_RLS_m(N, &j);

			SD_RLS_r* sd7 = new SD_RLS_r(N, &j);

			SD_RLS_STAR* sd8 = new SD_RLS_STAR(N, &j);

			SD_OnePlusOne* sd9 = new SD_OnePlusOne(N, &j);

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

	for(auto& ea : toDeleteEvolutionaryAlgorithms)
	{
		delete ea;
	}


	OpenGL og;
	og.Setup();
	og.Draw();
	og.ShutDown();
}
