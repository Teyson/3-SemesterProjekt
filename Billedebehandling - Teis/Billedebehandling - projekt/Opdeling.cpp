#include "Opdeling.h"



Opdeler::Opdeler()
{
}

Opdeler::Opdeler(std::string in)
{
    inputString = in;
}

std::vector<std::string> Opdeler::opdel(int o)
{
    opdelingsSize = o;
    int antalOpdelinger;
    std::string acc;

    antalOpdelinger = inputString.size() / opdelingsSize;
    stringVector.push_back(std::to_string(antalOpdelinger));

    if (antalOpdelinger > 0) {
        for (int i = 0; i < antalOpdelinger; i++)
        {
            acc = inputString.substr((i * opdelingsSize), opdelingsSize);

            //Strengene pushes til en vektor
            stringVector.push_back(acc);
        }
    }
    return stringVector;
}


Opdeler::~Opdeler()
{
}
