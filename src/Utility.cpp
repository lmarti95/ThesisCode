#include "Utilityh.h"

long long NoverK(int aN, int aK)
{
	if(aK == 0)
	{
		return 1;
	}
	else
	{
		return (aN * NoverK(aN - 1, aK - 1)) / aK;
	}
}

int* SelectRandomK(int aN, int aK)
{
	int* selection= new int[aK];

	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> randomN(1, aN);

	selection[0] = randomN(rng)-1;

	for(int i = 1; i < aK; ++i)
	{
		int random;
		bool unique = false;
		while(!unique)
		{
			random = randomN(rng) - 1;
			unique = true;

			for(int j = 0; j < i; ++j)
			{
				if(selection[j] == random)
				{
					unique = false;
				}
			}
		}

		selection[i] = random;
	}

	return selection;
}