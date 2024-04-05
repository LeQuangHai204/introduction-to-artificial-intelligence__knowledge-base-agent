#pragma once

#include <algorithm>

#include "KnowledgeModel.h"

class TruthTable
{
private:
	const KnowledgeModel* knowledgeModel;
	size_t count;

	void printHeader()
	{
		std::cout 
			<< "----------------------------------------------------------"
			<< "---------------------------------------------------------\n ";
		for (const Sentence* sentence : knowledgeModel->atomicSentences)
		{
			std::cout << sentence->getDescription() << "  ";
		}
		std::cout << "  ";

		for (const Sentence* sentence : knowledgeModel->completeSentences)
		{
			std::cout << sentence->getDescription() << "  ";
		}

		std::cout << "  query:" << knowledgeModel->query->getDescription()
			<< "\n----------------------------------------------------------"
			<< "---------------------------------------------------------\n";
	}

	void printRecord() 
	{
		std::cout << " ";
		for (const Sentence* sentence : knowledgeModel->atomicSentences)
		{
			printValue(sentence);
		} 
		std::cout << "  ";

		for (const Sentence* sentence : knowledgeModel->completeSentences)
		{
			printValue(sentence);
		}

		std::cout << "     ";
		printValue(knowledgeModel->query);
		std::cout << "   |\n";
	}

	void printValue(const Sentence* sentence)
	{
		size_t descSize = sentence->getDescription().size();
		size_t frontSize = descSize == 1 ? 0
			: descSize % 2
			? descSize / 2 - 1
			: descSize / 2;

		std::cout << std::string(frontSize, ' ')
			<< sentence->evaluate()
			<< std::string(descSize - frontSize + 1, ' ');
	}

	bool checkTable(std::vector<int> symbols, size_t index)
	{
		// set value for atomic symbols
		size_t current = 0;
		for (const Sentence* sentence : knowledgeModel->atomicSentences)
		{
			switch (symbols[current++])
			{
			case 1: sentence->setValue(true); break;
			case 0: sentence->setValue(false); break;
			case -1: break;
			default: throw std::logic_error("Unexpected value detected");
			}
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

			if (knowledgeModel->query->evaluate() == true)
			{
				count++;
				return true;
			}
			return false;
		}
		
		symbols[index] = 1;
		bool trueCase = checkTable(symbols, index + 1);
		
		symbols[index] = 0;
		bool falseCase = checkTable(symbols, index + 1);

		return trueCase && falseCase;
	}

public:
	TruthTable(const KnowledgeModel& knowledgeModel) : 
		knowledgeModel(&knowledgeModel), count(0)
	{ }

	bool execute()
	{
		printHeader();
		return checkTable(std::vector<int>(knowledgeModel->atomicSentences.size(), -1), 0);
	}

	size_t getCount()
	{
		return count;
	}
};