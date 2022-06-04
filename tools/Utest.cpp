#include <algorithm> 
#include <cmath>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>

struct Data{
    long number;
    int category;
};

std::vector<Data> datas;

double T1 = 0;
double T2 = 0;

double n1 = 100;
double n2 = 100;

void ReadIn(std::string aFilename, int aCategory)
{
    int lines;
    if(aCategory == 1)
    {
        lines = n1;
    }
    else
    {
        lines = n2;
    }


    std::ifstream file(aFilename);
    for(int i = 0; i < lines; ++i)
    {
        Data d;
        file >> d.number;
        d.category = aCategory;
        datas.push_back(d);
    }
}



int main()
{
    ReadIn("SD-RLS_Jump(2) Original_400.result", 1);
    ReadIn("SD-RLS-STAR_Jump(2) Original_400.result", 2);

    std::sort(datas.begin(), datas.end(), [](Data d1, Data d2)
        {
            return d1.number < d2.number;
        });

    for(int i = 0; i < n1 + n2; ++i)
    {
        if(datas.at(i).category == 1)
        {
            T1 += i;
        }
        else
        {
            T2 += i;
        }
    }

    std::cout << "T1: \t" << T1 << std::endl;
    std::cout << "T2: \t" << T2 << std::endl;

    double U1 = n1 * n2 + (n1 * (n1 + 1)) / 2 - T1;
    double U2 = n1 * n2 + (n2 * (n2 + 1)) / 2 - T2;

    std::cout << "U1: \t" << U1 << std::endl;
    std::cout << "U2: \t" << U2 << std::endl;

    double U;
    if(U1 < U2)
    {
        U = U1;
    }
    else
    {
        U = U2;
    }

    std::cout << "U: \t" << U << std::endl;

    double MuU = n1 * n2 / 2;
    std::cout << "MuU: \t" << MuU << std::endl;

    double SigmaU = std::sqrt((n1 * n2 * (n1 + n2 + 1) / 12));

    std::cout << "SigmaU: " << SigmaU << std::endl;

    double z = (U - MuU) / SigmaU;

    std::cout << "z: \t" << z << std::endl;

}