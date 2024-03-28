#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <regex>
#include <unordered_set>
#include <stack>

#include "Index.h"

std::unordered_set<Sentence> tokenize(const std::string& str)
{
    std::unordered_set<Sentence> sentences;
    size_t prev = 0;
    size_t pos;
    while ((pos = str.find_first_of("~", prev)) != std::string::npos)
    {
        if (pos > prev)
        {
            std::string token = str.substr(prev, pos - prev);
            sentences.emplace(token);
        }
        prev = pos + 1;
    }

    if (prev < str.length())
    {
        std::string lastToken = str.substr(prev, std::string::npos);
        sentences.emplace(lastToken);
    }

    return sentences;
}


int main(int argc, char* argv[])
{
#ifdef RUN_ON_VISUALSTUDIO
    std::ifstream inputFile("test_genericKB.txt");
#endif // RUN_ON_VISUALSTUDIO

#ifdef RUN_ON_TERMINAL
    if (argc < 3)
    {
        std::cerr << "Arguments missing." << std::endl;
        return 1;
    }

    std::ifstream inputFile(argv[2]);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }
#endif // RUN_ON_TERMINAL

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
            part.erase(std::remove_if(
                part.begin(), 
                part.end(), 
                [](char ch) { return std::isspace(static_cast<unsigned char>(ch)); }), 
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
    query.erase(std::remove_if(
        query.begin(),
        query.end(),
        [](char ch) { return std::isspace(static_cast<unsigned char>(ch)); }),
        query.end());

#ifdef DEVELOP
    std::cout << "Extracted Clauses:" << std::endl;
    for (const std::string& clause : knowledge) {
        std::cout << clause << std::endl;
    }
    std::cout << "Extracted Query:" << std::endl;        
    std::cout << query << std::endl;
#endif // DEVELOP


}

