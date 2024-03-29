#pragma once

#include "Symbol.h"
#include "Constants.h"

class Imply : public Symbol
{
private:
	Sentence* antecedent;
	Sentence* consequence;

public:
	Imply(Sentence* antecedent, Sentence* consequence) :
		antecedent(antecedent), consequence(consequence)
	{
		if (!antecedent || !consequence)
		{
			throw std::invalid_argument("Null sentence in Imply constructor");
		}
	}

	std::string getDescription() const override
	{
		return (antecedent->isSymbol() ? "(" : "") 
			+ antecedent->getDescription()
			+ (antecedent->isSymbol() ? ")" : "")
			+ " => " 
			+ (consequence->isSymbol() ? "(" : "")
			+ consequence->getDescription()
			+ (consequence->isSymbol() ? ")" : "");
	}

	bool getValue() const override
	{
		return !antecedent->getValue() || consequence->getValue();
	}

	bool operator==(const Sentence& other) const override
	{
		const Imply* implySymbol = dynamic_cast<const Imply*>(&other);
		return implySymbol && operator==(*implySymbol);
	}

	bool operator==(const Imply& other) const
	{
		return antecedent == other.antecedent 
			&& consequence == other.consequence;
	}
};
