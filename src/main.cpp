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
#include "SD_RLS.h"
#include "SD_RLS_STAR.h"
#include "TwoPlusOneGA.h"


int main()
{
	int N = 25;
	Jump j(N,3);

	SD_RLS sd(N, &j);

	SD_RLS_STAR sd2(N, &j);

	Benchmark b;

	b.RunEA(&sd);
	b.RunEA(&sd2);
}