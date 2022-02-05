#include "Benchmark.h"
#include "cGA.h"
#include "Cliff.h"
#include "CostFunction.h"
#include "HybridGA.h"
#include "Jump.h"
#include "MuPlusOneGA.h"
#include "SASD_OnePlusLambda.h"
#include "SDOnePlusOne.h"
#include "SD_RLS.h"
#include "SD_RLS_m.h"
#include "SD_RLS_r.h"
#include "SD_RLS_STAR.h"


int main()
{
	int N = 30;
	Jump j(N,3);

	Benchmark b;

	cGA sd(N, &j, 4);

	HybridGA sd2(N, &j);

	MuPlusOneGA sd3(N, &j, 4);

	SASD_OnePlusLambda sd4(N, &j, 4);

	SD_RLS sd5(N, &j);

	SD_RLS_m sd6(N, &j);

	SD_RLS_r sd7(N, &j);

	SD_RLS_STAR sd8(N, &j);

	SDOnePlusOne sd9(N, &j);

	b.RunEA(&sd);
	b.RunEA(&sd2);
	b.RunEA(&sd3);
	b.RunEA(&sd4);
	b.RunEA(&sd5);
	b.RunEA(&sd6);
	b.RunEA(&sd7);
	b.RunEA(&sd8);
	b.RunEA(&sd9);
}