#include "Benchmark.h"
#include "cGA.h"
#include "Cliff.h"
#include "CostFunction.h"
#include "HybridGA.h"
#include "Jump.h"
#include "MST.h"
#include "MuPlusOneGA.h"
#include "SASD_OnePlusLambda.h"
#include "SDOnePlusOne.h"
#include "SD_RLS.h"
#include "SD_RLS_m.h"
#include "SD_RLS_r.h"
#include "SD_RLS_STAR.h"

#include <iostream>

int main()
{
	std::cout << "Maximum number of threads used: " << std::thread::hardware_concurrency() << std::endl;

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

	for(auto& N : testNs)
	{
		for(int gapSize = 2; gapSize < 7; ++gapSize)
		{
			if(N / 3 < gapSize)
			{
				break;
			}

			Jump j(N, 3, JumpType::Original);

			//MST j(N, "7_21.mst");

			Benchmark b;
			b.SetRepeat(100);

			cGA sd(N, &j, 4);

			HybridGA sd2(N, &j);

			MuPlusOneGA sd3(N, &j, 4);

			SASD_OnePlusLambda sd4(N, &j, 4);

			SD_RLS sd5(N, &j);

			SD_RLS_m sd6(N, &j);

			SD_RLS_r sd7(N, &j);

			SD_RLS_STAR sd8(N, &j);

			SDOnePlusOne sd9(N, &j);

			b.ScheduleEA(&sd);
			b.ScheduleEA(&sd2);
			b.ScheduleEA(&sd3);
			b.ScheduleEA(&sd4);
			b.ScheduleEA(&sd5);
			b.ScheduleEA(&sd6);
			b.ScheduleEA(&sd7);
			b.ScheduleEA(&sd8);
			b.ScheduleEA(&sd9);

			b.SaveResults("Jump(" + std::to_string(gapSize) + ").txt");

		}
	}	
}
