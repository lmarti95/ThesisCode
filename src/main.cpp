#include "Benchmark.h"
#include "cGA.h"
#include "Cliff.h"
#include "CostFunction.h"
#include "HybridGA.h"
#include "Jump.h"
#include "LeadingOnes.h"
#include "MuPlusOneEA.h"
#include "MuPlusOneGA.h"
#include "OneCommaLambdaEA.h"
#include "OneMax.h"
#include "OnePlusLambdaCommaLambdaGA.h"
#include "OnePlusOneEA.h"
#include "SASD_OnePlusLambda.h"
#include "SDOnePlusOne.h"
#include "SD_RLS.h"
#include "SD_RLS_m.h"
#include "SD_RLS_r.h"
#include "SD_RLS_STAR.h"
#include "TwoPlusOneGA.h"


int main()
{
	int N = 12;
	Jump j(N,3);

	Benchmark b;

	SD_RLS sd(N, &j);

	SD_RLS_m sd2(N, &j);

	SD_RLS_r sd3(N, &j);

	SD_RLS_STAR sd4(N, &j);

	SASD_OnePlusLambda sd5(N, &j, 10);

	SDOnePlusOne sd6(N, &j);

	b.RunEA(&sd);
	b.RunEA(&sd2);
	b.RunEA(&sd3);
	b.RunEA(&sd4);
	b.RunEA(&sd5);
	b.RunEA(&sd6);
}