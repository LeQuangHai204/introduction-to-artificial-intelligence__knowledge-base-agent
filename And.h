#pragma once

#include <vector>

#include "Symbol.h"
#include "Constants.h"

class And : public Symbol
{
public:
	And(std::vector<Sentence*>&& sentences) : 
		Symbol(std::move(sentences)) 
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

	bool equals(const Symbol& other) const override
	{
		const And* andSymbol = dynamic_cast<const And*>(&other);
		if (!andSymbol)
		{
			return false;
		}

		for (Sentence* sentence1 : sentences)
		{
			bool found = false;
			for (Sentence* sentence2 : andSymbol->sentences)
			{
				if (sentence1 == sentence2) 
				{
					found = true;
					break;
				}
			}

			if (!found)
			{
				return false;
			}
		}
		
		return true;
	}
};
