#pragma once

#include "Symbol.h"
#include "Constants.h"

class Not : public Symbol
{
private:
	Sentence* sentence;

public:
	Not(Sentence* sentence) : sentence(sentence) 
	{
		if (!sentence)
		{
			throw std::invalid_argument("Null sentence in Not constructor");
		}
	}

	std::string getDescription() const override
	{
		if (sentence->isSymbol()) return
			"~ (" + sentence->getDescription() + ")";
		return "~ " + sentence->getDescription();
	}

	bool getValue() const override
	{
		return !sentence->getValue();
	}

	bool operator==(const Sentence& other) const override
	{
		const Not* notSymbol = dynamic_cast<const Not*>(&other);
		return notSymbol && operator==(*notSymbol);
	}

	bool operator==(const Not& other) const
	{
		return sentence == other.sentence;
	}
};
