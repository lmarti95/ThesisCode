#include "MST.h"

#include <cmath>
#include <fstream>
#include <iostream>

std::mutex MST::mGetEdgeMutex;

MST::MST(int aN, std::string aFilename) : CostFunction(aN)
{
	ReadIn(aFilename);

    if(mEdgesNum != mN)
    {
        std::cout << "The number of edges and the length of the bitstring is not equal!" << std::endl;
        exit(-1);
    }
}

void MST::ReadIn(std::string aFilename)
{
    std::ifstream file(aFilename);
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
	return 1/(double)mMinimumSum;
}

void MST::visitEdge(int* aBitString, int aEdge, int* aConnected)
{
    if(aConnected[mEdges.at(aEdge).n1] == 0)
    {
        aConnected[mEdges.at(aEdge).n1] = 1;
        for(int i = 0; i < mEdgesNum; ++i)
        {
            if(aBitString[i] == 1 && i != aEdge)
            {
                if(mEdges.at(i).n1 == mEdges.at(aEdge).n1 || mEdges.at(i).n2 == mEdges.at(aEdge).n1)
                {
                    visitEdge(aBitString, i, aConnected);
                }
            }
        }
    }

    if(aConnected[mEdges.at(aEdge).n2] == 0)
    {
        aConnected[mEdges.at(aEdge).n2] = 1;
        for(int i = 0; i < mEdgesNum; ++i)
        {
            if(aBitString[i] == 1 && i != aEdge)
            {
                if(mEdges.at(i).n1 == mEdges.at(aEdge).n2 || mEdges.at(i).n2 == mEdges.at(aEdge).n2)
                {
                    visitEdge(aBitString, i, aConnected);
                }
            }
        }
    }
}

int MST::ConnectedComponents(int* aBitString)
{
    int components = 0;
    int* connected = new int[mNodesNum];

    for(int i = 0; i < mNodesNum; ++i)
    {
        connected[i] = 0;
    }

    for(int i = 0; i < mNodesNum; ++i)
    {
        if(connected[i] == 1)
        {
            continue;
        }

        int found = -1;

        for(int j = 0; j < mEdgesNum; ++j)
        {
            if(aBitString[j] == 1)
            {
                if(mEdges.at(j).n1 == i || mEdges.at(j).n2 == i)
                {
                    found = j;
                    break;
                }
            }
        }

        if(found != -1)
        {
            components++;
            visitEdge(aBitString, found, connected);
        }
        else
        {
            components++;
            connected[i] = 1;
        }
    }

    delete[] connected;

    return components;
}

double MST::GetFitnessValue(int* aBitString)
{
    int components = ConnectedComponents(aBitString);

    double weightSum = 0;
    int edgesSum = 0;

    for(int i = 0; i < mN; ++i)
    {
        if(aBitString[i] == 1)
        {
            weightSum += mEdges.at(i).weight;
            edgesSum++;
        }
    }

    return  1 / (((double)(components - 1) * std::pow(mM, 2)) + ((double)(edgesSum - (mNodesNum - 1)) * mM) + weightSum);
}
