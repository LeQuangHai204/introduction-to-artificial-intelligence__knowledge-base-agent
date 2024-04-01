#pragma once

#include <utility>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "Sentence.h"

class FileReader
{
public:
	static std::pair<std::vector<std::string>, std::string> readFile(int argc, char* argv[])
	{
#ifdef RUN_ON_VISUALSTUDIO
            std::ifstream inputFile("test_HornKB.txt");
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

        return std::make_pair(knowledge, query);
	}
};