#include "GeneticAlgorithm.h"

GeneticAlgorithm::GeneticAlgorithm(int N, CostFunction* aCostFunction) : EvolutionaryAlgorithm(N, aCostFunction)
{
}

int* GeneticAlgorithm::CreateMutatedOffSpringUniformCrossover(std::vector<std::pair<int*, double>> aParents)
{
	int* bitString = new int[mN];

	std::uniform_int_distribution<std::mt19937::result_type> parentsRandom(0, aParents.size()-1);

	for(unsigned int i = 0; i <mN; ++i)
	{
		bitString[i] = aParents.at(parentsRandom(mRng)).first[i];
	}

	return bitString;
}
