#pragma once

#include <algorithm>
#include <cmath>

#include "KnowledgeModel.h"

class TruthTable
{
private:
	const KnowledgeModel* knowledgeModel;

	void printHeader()
	{
		std::cout << "----------------------------------------------------------"
			<< "-----------------------------\n ";
		for (const Sentence* sentence : knowledgeModel->atomicSentences)
		{
			std::cout << sentence->getDescription() << "   ";
		}
		std::cout << "    ";

		for (const Sentence* sentence : knowledgeModel->completeSentences)
		{
			std::cout << sentence->getDescription() << "   ";
		}

		std::cout << "    query:" << knowledgeModel->query->getDescription()
			<< "\n----------------------------------------------------------"
			<< "-----------------------------\n";
	}

	void printRecord() 
	{
		std::cout << ' ';
		for (const Sentence* sentence : knowledgeModel->atomicSentences)
		{
			std::cout << sentence->evaluate() << "   ";
		} 
		std::cout << "    ";

		for (const Sentence* sentence : knowledgeModel->completeSentences)
		{
			size_t descSize = sentence->getDescription().size();
			std::cout << std::string(descSize / 3, ' ')
				<< sentence->evaluate()
				<< std::string(descSize - descSize / 3 - 1, ' ');
		}

		size_t queryDescSize = knowledgeModel->query->getDescription().size();
		std::cout << "\b\b     " << std::string(queryDescSize / 3, ' ')
			<< knowledgeModel->query->evaluate()
			<< std::string(queryDescSize - queryDescSize / 3 - 1, ' ')
			<< "      |\n";
	}

	bool checkTable(std::vector<int> symbols, size_t index)
	{
		// set value for atomic symbols
		size_t current = 0;
		for (const Sentence* sentence : knowledgeModel->atomicSentences)
		{
			switch (symbols[current])
			{
			case 1: sentence->setValue(true); break;
			case 0: sentence->setValue(false); break;
			case -1: break;
			default: throw std::logic_error("Unexpected value detected");
			}
			current++;
		}

		// recursive base case
		if (index == symbols.size())
		{
			printRecord();

			for (const Sentence* sentence : knowledgeModel->completeSentences)
			{
				if (!sentence->evaluate()) 
				{
					return true;
				}
			}

			return knowledgeModel->query->evaluate();
		}
		
		symbols[index] = 1;
		bool trueCase = checkTable(symbols, index + 1);
		
		symbols[index] = 0;
		bool falseCase = checkTable(symbols, index + 1);

		return trueCase && falseCase;
	}

public:
	TruthTable(const KnowledgeModel& knowledgeModel) : 
		knowledgeModel(&knowledgeModel)
	{ }

	bool execute()
	{
		printHeader();
		return checkTable(std::vector<int>(knowledgeModel->atomicSentences.size(), -1), 0);
	}
};