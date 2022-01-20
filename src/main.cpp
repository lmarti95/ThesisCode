#include "Benchmark.h"
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
#include "SD_RLS_STAR.h"
#include "TwoPlusOneGA.h"


int main()
{
	int N = 12;
	Jump j(N,3);

	SDOnePlusOne sd(N, &j);

	SASD_OnePlusLambda sd2(N, &j,4);

	Benchmark b;

	b.RunEA(&sd);
	b.RunEA(&sd2);
}