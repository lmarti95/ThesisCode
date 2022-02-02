#include "Benchmark.h"
#include "cGA.h"
#include "Cliff.h"
#include "CostFunction.h"
#include "Jump.h"
#include "LeadingOnes.h"
#include "MuPlusOneEA.h"
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
	int N = 10;
	Jump j(N,5);

	cGA sd3(N, &j, 4);
	Benchmark b;

	/*SD_RLS_m sd(N, &j);

	SD_RLS_r sd2(N, &j);

	SD_RLS sd3(N, &j);

	b.RunEA(&sd);
	b.RunEA(&sd2);*/
	b.RunEA(&sd3);
}