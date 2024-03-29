#pragma once

#include <vector>

#include "Symbol.h"
#include "Constants.h"

class And : public Symbol
{
private:
	std::set<Sentence*> sentences;

public:
	And(std::set<Sentence*>&& sentences) : 
		sentences(std::move(sentences)) 
	{
		if (this->sentences.size() < 2) throw std::domain_error(
			"And clause requires at least 2 sentences");
	}

	std::string getDescription() const override
	{
		std::string result = "";
		for (Sentence* sentence : sentences)
		{
			if (!sentence) throw std::domain_error(
				"Symbol contain NULL sentence");
			if (sentence->isSymbol()) result += "(";
			result += sentence->getDescription();
			if (sentence->isSymbol()) result += ")";
			result += " && ";
		}

		result.erase(result.size() - 4);
		return result;
	}

	bool getValue() const override
	{
		for (Sentence* sentence : sentences)
		{
			if (!sentence->getValue())
			{
				return false;
			}
		}

		return true;
	}

	bool operator==(const Sentence& other) const override
	{
		const And* andSymbol = dynamic_cast<const And*>(&other);
		return andSymbol && operator==(*andSymbol);
	}

	bool operator==(const And& other) const
	{
		return sentences == other.sentences;
	}
};
