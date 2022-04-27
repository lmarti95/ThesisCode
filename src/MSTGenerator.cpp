#include "MSTGenerator.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>
#include <string>

bool MSTGenerator::CheckConnected(std::vector<Edge>* aVectors, int aNodes)
{
    bool returnValue = true;

    int* connected = new int[aNodes];

    for(int i = 0; i < aNodes; ++i)
    {
        connected[i] = 0;
    }

    std::vector<int> visited;
    std::vector<int> visitNext;
    visitNext.push_back(aVectors->at(0).n1);

    while(visitNext.size() != 0)
    {
        int visit = visitNext.at(0);
        for(auto& v : *aVectors)
        {
            if(v.n1 == visit)
            {
                if(connected[v.n2] == 0)
                {
                    visitNext.push_back(v.n2);
                    connected[v.n2] = 1;
                }
            }
            if(v.n1 == visit)
            {
                if(connected[v.n1] == 0)
                {
                    visitNext.push_back(v.n1);
                    connected[v.n1] = 1;
                }
            }
        }

        visitNext.erase(visitNext.begin());
        visited.push_back(visit);
    }

    for(int i = 0; i < aNodes; ++i)
    {
        if(connected[i] == 0)
        {
            returnValue = false;
        }
    }

    delete[] connected;

    return returnValue;
}

void MSTGenerator::SetAvailable(int* aAvailable, int aNodes)
{
    for(int i = 0; i < aNodes; ++i)
    {
        aAvailable[i] = 1;
    }
}

std::vector<Edge>* MSTGenerator::GenerateVectors(int aNodes, int aEdges)
{
    std::uniform_int_distribution<std::mt19937::result_type> randomN(0, aNodes - 1);
    std::uniform_int_distribution<std::mt19937::result_type> randomEdge(0, aEdges - 1);
    std::random_device dev;
    std::mt19937 rng(dev());

    std::vector<Edge>* selectedEdges = new std::vector<Edge>;
    std::vector<Edge> possibleEdges;

    for(int i = 0; i < aNodes; ++i)
    {
        for(int j = i + 1; j < aNodes; ++j)
        {
            Edge e;
            e.n1 = i;
            e.n2 = j;
            possibleEdges.push_back(e);
        }
    }

    std::shuffle(possibleEdges.begin(), possibleEdges.end(), rng);

    int* found = new int[aNodes];
    for(int i = 0; i < aNodes; ++i)
    {
        found[i] = 0;
    }
    for(int i = 0; i < aNodes; ++i)
    {
        if(found[i] == 1)
        {
            continue;
        }

        for(unsigned int j = 0; j < possibleEdges.size(); ++j)
        {
            if(i == possibleEdges.at(j).n1 || i == possibleEdges.at(j).n2)
            {
                Edge e = possibleEdges.at(j);
                found[e.n1] = 1;
                found[e.n2] = 1;
                selectedEdges->push_back(e);
                possibleEdges.erase(possibleEdges.begin() + j);
                j--;
                break;
            }
        }
    }

    int i = 0;
    while(selectedEdges->size() != aEdges)
    {
        selectedEdges->push_back(possibleEdges.at(i));
        i++;
    }

    delete[] found;

    return selectedEdges;
}

std::vector<Edge>* MSTGenerator::GenerateGraph(int aNodes, int aEdges)
{
    std::vector<Edge>* vectors = GenerateVectors(aNodes, aEdges);

    while(!CheckConnected(vectors, aNodes))
    {
        delete vectors;
        vectors = GenerateVectors(aNodes, aEdges);
    }

    return vectors;
}

void MSTGenerator::GenerateWeights(std::vector<Edge>* aEdges, int aFrom, int aTo)
{
    std::uniform_int_distribution<std::mt19937::result_type> randomWeight(aFrom, aTo);
    std::random_device dev;
    std::mt19937 rng(dev());

    for(unsigned int i = 0; i < aEdges->size(); ++i)
    {
        aEdges->at(i).weight = randomWeight(rng);
    }
}

int MSTGenerator::CalculateLowestMSTWeight(std::vector<Edge>* aEdges, int aNodesNum)
{
    std::sort(aEdges->begin(), aEdges->end(), [](Edge e1, Edge e2)
    {
        return e1.weight < e2.weight;
    });

    std::vector<int> added;

    int sum = 0;

    added.push_back(aEdges->at(0).n1);
    added.push_back(aEdges->at(0).n2);
    sum += aEdges->at(0).weight;

    while(added.size() != aNodesNum)
    {
        for(auto& e : *aEdges)
        {
            bool n1 = !(std::find(added.begin(), added.end(), e.n1) == added.end());
            bool n2 = !(std::find(added.begin(), added.end(), e.n2) == added.end());

            if(n1 && n2)
            {
                continue;
            }

            if(!n2 && n1)
            {
                added.push_back(e.n2);
                sum += e.weight;
                break;
            }

            if(!n1 && n2)
            {
                added.push_back(e.n1);
                sum += e.weight;
                break;
            }
        }
    }

    return sum;
}

void MSTGenerator::CreateMST(int aNodes, int aEdges)
{
    std::string filename = std::to_string(aNodes) + "_" + std::to_string(aEdges) + ".mst";
    std::ifstream file(filename);
    if(file.good())
    {
        return;
    }

   std::vector<Edge>* edges = GenerateGraph(aNodes, aEdges);

    GenerateWeights(edges, 0, 100);

    SaveMST(edges, aNodes, aEdges);

    delete edges;
}

void MSTGenerator::SaveMST(std::vector<Edge>* aEdges, int aNodes, int aEdgesNum)
{
    int sum = CalculateLowestMSTWeight(aEdges, aNodes);

    std::ofstream file;
    std::string mst = ".mst";
    std::string filename = std::to_string(aNodes) + "_" + std::to_string(aEdgesNum) + mst;
    file.open(filename);
    file << aNodes << std::endl;
    file << aEdgesNum << std::endl;
    file << sum << std::endl;

    std::random_device dev;
    std::mt19937 rng(dev());

    std::shuffle(aEdges->begin(), aEdges->end(), rng);

    for(auto& e : *aEdges)
    {
        file << e.n1 << "\t" << e.n2 << "\t" << e.weight << std::endl;
    }
    file.close();
}