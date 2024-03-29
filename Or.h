#pragma once

#include <set>

#include "Symbol.h"
#include "Constants.h"

class Or : public Symbol
{
private:
	std::set<Sentence*> sentences;
public:
	Or(std::set<Sentence*>&& sentences) : 
		sentences(std::move(sentences))
	{
		if (this->sentences.size() < 2) 
		{
			throw std::invalid_argument("Or clause requires at least 2 sentences");
		}
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
			result += " || ";
		}

		result.erase(result.size() - 4);
		return result;
	}

	bool getValue() const override
	{
		for (Sentence* sentence : sentences)
		{
			if (sentence->getValue())
			{
				return true;
			}
		}

		return false;
	}

	bool operator==(const Sentence& other) const override
	{
		const Or* orSymbol = dynamic_cast<const Or*>(&other);
		return orSymbol && operator==(*orSymbol);
	}

	bool operator==(const Or& other) const
	{
		return sentences == other.sentences;
	}
};
