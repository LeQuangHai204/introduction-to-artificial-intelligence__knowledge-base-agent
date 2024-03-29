#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>

#include "Constants.h"
#include "Index.h"

int main(int argc, char* argv[])
{
#ifdef TEST
    Sentence* pureSen0 = new Sentence("It rains");
    Sentence* pureSen1 = new Sentence("It is cloudy");
    Sentence* and0 = new And({pureSen0, pureSen1});
    Sentence* and1 = new And({pureSen1, pureSen0});
    Sentence* or0 = new Or({pureSen0, pureSen1});
    Sentence* or1 = new Or({pureSen1, pureSen0});
    Sentence* not0 = new Not(pureSen0);
    Sentence* not1 = new Not(pureSen1);
    std::cout << "Compare: " << ((*or0) == (*or1));
    return 0;
#endif  // TEST

#ifdef RUN_ON_VISUALSTUDIO
    std::ifstream inputFile("test_genericKB.txt");
#endif  // RUN_ON_VISUALSTUDIO

#ifdef RUN_ON_TERMINAL
    if (argc < 3)
    {
        std::cerr << "Arguments missing." << std::endl;
        return 1;
    }

    std::ifstream inputFile(argv[2]);
    if (!inputFile.is_open())
    {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }
#endif  // RUN_ON_TERMINAL

    std::string line;
    std::vector<std::string> knowledge;
    std::string query;
    while (std::getline(inputFile, line))
    {
        if (line != "TELL")
        {
            continue;
        }
        if (!std::getline(inputFile, line))
        {
            throw std::ios_base::failure("End of File reached.");
        }

        std::stringstream ss(line);
        std::string part;
        while (std::getline(ss, part, ';'))
        {
            // Remove all spaces in clause
            part.erase(
                std::remove_if(part.begin(),
                    part.end(),
                    [](char ch)
                    { return std::isspace(static_cast<unsigned char>(ch)); }),
                part.end());

            if (!part.empty()) knowledge.push_back(part);
        }
        break;
    }
    while (std::getline(inputFile, line))
    {
        if (line != "ASK")
        {
            continue;
        }
        if (!std::getline(inputFile, query))
        {
            throw std::ios_base::failure("End of File reached.");
        }
        break;
    }

    // Remove all spaces in query
    query.erase(std::remove_if(query.begin(),
                    query.end(),
                    [](char ch)
                    { return std::isspace(static_cast<unsigned char>(ch)); }),
        query.end());

#ifdef DEVELOP

    std::cout << "Extracted Clauses:" << std::endl;
    for (const std::string& clause : knowledge)
    {
        std::cout << clause << std::endl;
        Sentence* sentence = StringToSymbol::convert(clause);
    }
    std::cout << "Extracted Query:" << std::endl;
    std::cout << query << std::endl;

    std::cout << "Atomic Knowledge:" << std::endl;
    for (const Sentence& elem : StringToSymbol::atomicSentences)
    {
        std::cout << elem.getDescription() << " ";
    }
#endif  // DEVELOP
}
