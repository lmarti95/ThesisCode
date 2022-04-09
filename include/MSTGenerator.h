#pragma once

#include <vector>

#include "MST.h"

class MSTGenerator
{
public:
    void CreateMST(int aNodes, int aEdges);

private:

    bool CheckConnected(std::vector<Edge>* aVectors, int aNodes);
    void SetAvailable(int* aAvailable, int aNodes);

    std::vector<Edge>* GenerateVectors(int aNodes, int aEdges);
    std::vector<Edge>* GenerateGraph(int aNodes, int aEdges);
    void GenerateWeights(std::vector<Edge>* aEdges, int aFrom, int aTo);

    int CalculateLowestMSTWeight(std::vector<Edge>* aEdges, int aNodesNum);

    void SaveMST(std::vector<Edge>* aEdges, int aNodes, int aEdgesNum);
};