#include "MST.h"

#include <fstream>
#include <iostream>

MST::MST(int aN, std::string aFilename) : CostFunction(aN)
{
	ReadIn(aFilename);
}

void MST::ReadIn(std::string aFilename)
{
    std::ifstream file("aFilename");
    if(file.is_open())
    {
        file >> mNodesNum;
        file >> mEdgesNum;
        file >> mMinimumSum;

        for(int i = 0; i < mEdgesNum; ++i)
        {
            Edge e;
            file >> e.n1 >> e.n2 >> e.weight;
            mEdges.push_back(e);
        }

        file.close();
    }
    else
    {
        std::cout << "File " << aFilename << " not found!" << std::endl;
        exit(-1);
    }
}

double MST::GetMaximumFitnessValue()
{
	return 0;
}

double MST::GetFitnessValue(int* aBitString)
{
	return 0;
}