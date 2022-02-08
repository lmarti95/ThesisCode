#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <utility>
#include <vector>

struct Edge{
    int n1;
    int n2;
    int weight;
};

bool CheckConnected(std::vector<Edge>* vectors, int N)
{
    bool returnValue = true;

    int* connected = new int[N];

    for(int i = 0; i < N; ++i)
    {
        connected[i] = 0;
    }

    std::vector<int> visited;
    std::vector<int> visitNext;
    visitNext.push_back(vectors->at(0).n1);

    while(visitNext.size() != 0)
    {
        int visit = visitNext.at(0);
        for(auto& v : *vectors)
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
    
    for(int i = 0; i < N; ++i)
    {
        if(connected[i] == 0)
        {
            returnValue = false;
        }
    }

    delete[] connected;

    return returnValue;
}

void SetAvailable(int* available, int N)
{
    for(int i = 0; i < N; ++i)
    {
        available[i] = 1;
    }
}

std::vector<Edge> GenerateVectors(int N, int edges)
{
    std::uniform_int_distribution<std::mt19937::result_type> randomN(0, N - 1);
    std::uniform_int_distribution<std::mt19937::result_type> randomEdge(0, edges - 1);
    std::random_device dev;
    std::mt19937 rng(dev());

    std::vector<Edge> vectors;
    std::vector<int> edgePossiblities;

    for(int i = 0; i < edges; ++i)
    {
        Edge e;
        e.n1 = -1;
        e.n2 = -1;
        vectors.push_back(e);
        edgePossiblities.push_back(i);
    }

    std::shuffle(edgePossiblities.begin(), edgePossiblities.end(), rng);

    for(int i = 0; i < N; ++i)
    {
        int edge;
        if(i > edges - 1)
        {
            edge = edgePossiblities.at(0);
        }
        else
        {
            edge = edgePossiblities.at(i);
        }

        while(vectors.at(edge).n1 != -1 && vectors.at(edge).n2 != -1)
        {
            edge = randomEdge(rng);
        }

        if(vectors.at(edge).n1 == -1)
        {
            vectors.at(edge).n1 = i;
            continue;
        }

        if(vectors.at(edge).n2 == -1)
        {
            vectors.at(edge).n2 = i;
            edgePossiblities.erase(edgePossiblities.begin());
        }
    }

    for(auto& v : vectors)
    {
        if(v.n1 == -1)
        {
            v.n1 = randomN(rng);
        }
    }

    int* available = new int[N];

    for(auto& v : vectors)
    {
        if(v.n2 == -1)
        {
            bool regenerate = true;
            while(regenerate)
            {
                regenerate = false;
                std::vector<int> availableVector;
                SetAvailable(available, N);
                available[v.n1] = 0;
                for(auto& v2 : vectors)
                {
                    if(v.n1 == v2.n1)
                    {
                        if(v2.n2 != -1)
                        {
                            available[v2.n2] = 0;
                        }
                    }
                    if(v.n1 == v2.n2)
                    {
                        available[v2.n1] = 0;
                    }
                }

                for(int i = 0; i < N; ++i)
                {
                    if(available[i] == 1)
                    {
                        availableVector.push_back(i);
                    }
                }

                std::shuffle(availableVector.begin(), availableVector.end(), rng);

                if(availableVector.size() == 0)
                {
                    regenerate = true;
                    v.n1 = randomN(rng);
                }
                else
                {
                    v.n2 = availableVector.at(0);
                }
                
            }
        }
    }

    delete[] available;

    return vectors;
}

std::vector<Edge> GenerateGraph(int N, int edges)
{
    if(edges < N - 1)
    {
        std::cout << "Too few edges specified!" << std::endl;
        exit(-1);
    }

    if(edges > N*(N - 1)/2)
    {
        std::cout << "Too many edges specified!" << std::endl;
        exit(-1);
    }

    std::vector<Edge> vectors = GenerateVectors(N, edges);

    while(!CheckConnected(&vectors, N))
    {
        vectors = GenerateVectors(N, edges);
    }

    return vectors;
}

void GenerateWeights(std::vector<Edge>* edges, int from, int to)
{
  
    std::uniform_int_distribution<std::mt19937::result_type> randomWeight(from, to);
    std::random_device dev;
    std::mt19937 rng(dev());

    for(unsigned int i = 0; i < edges->size(); ++i)
    {
        edges->at(i).weight = randomWeight(rng);
    }
}

int CalculateLowestMSTWeight(std::vector<Edge>* edges)
{
    std::sort(edges->begin(), edges->end(), [](Edge e1, Edge e2)
    {
        return e1.weight < e2.weight;
    });

    std::vector<int> added;

    int sum = 0;

    for(auto& e : *edges)
    {
        bool n1 = !(std::find(added.begin(), added.end(), e.n1) == added.end());
        bool n2 = !(std::find(added.begin(), added.end(), e.n2) == added.end());

        if(n1 && n2)
        {
            continue;
        }

        if(!n2)
        {
            added.push_back(e.n2);
        }

        if(!n1)
        {
            added.push_back(e.n1);
        }

        sum += e.weight;
    }

    return sum;
}

int main()
{
    int nodesNum = 10;
    int edgesNum = 9;
    std::vector<Edge> edges = GenerateGraph(nodesNum, edgesNum);

    GenerateWeights(&edges, 0, 100);

    for(auto& e : edges)
    {
        std::cout << e.n1 << "\t" << e.n2 << "\t" << e.weight << std::endl;
    }

    int sum = CalculateLowestMSTWeight(&edges);

    std::cout << "Minimum sum: " << sum << std::endl;

    std::ofstream file;
    std::string mst = ".mst";
    std::string filename = std::to_string(nodesNum) + "_" + std::to_string(edgesNum) + mst;
    file.open(filename);
    file << nodesNum << std::endl;
    file << edgesNum << std::endl;
    file << sum << std::endl;

    for(auto& e : edges)
    {
        file << e.n1 << "\t" << e.n2 << "\t" << e.weight << std::endl;
    }
    file.close();
}