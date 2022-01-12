#include "Benchmark.h"
#include "Cliff.h"
#include "CostFunction.h"
#include "Jump.h"
#include "LeadingOnes.h"
#include "OneCommaLambdaEA.h"
#include "OneMax.h"
#include "OnePlusLambdaCommaLambdaGA.h"
#include "OnePlusOneEA.h"
#include "MuPlusOneEA.h"
#include "TwoPlusOneGA.h"

int main()
{
	int N = 1000;
	OneMax om(N);

	OnePlusOneEA opo(N, &om);

	Benchmark b;

	b.RunEA(&opo);
}