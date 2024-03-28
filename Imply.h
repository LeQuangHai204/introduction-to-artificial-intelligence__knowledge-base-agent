#pragma once

#include "Symbol.h"
#include "Constants.h"

class Imply : public Symbol
{
public:
	Imply(Sentence* antecedent, Sentence* consequence) : 
		Symbol(std::vector{ antecedent, consequence }) { }

	std::string getDescription() const override
	{
		return (sentences[0]->isSymbol() ? "(" : "") 
			+ sentences[0]->getDescription()
			+ (sentences[0]->isSymbol() ? ")" : "")
			+ " => " 
			+ (sentences[1]->isSymbol() ? "(" : "")
			+ sentences[1]->getDescription()
			+ (sentences[1]->isSymbol() ? ")" : "");
	}

	bool getValue() const override
	{
		return !sentences[0]->getValue() || sentences[1]->getValue();
	}

	bool equals(const Symbol& other) const override
	{
		if (dynamic_cast<const Imply*>(&other))
		{
			return Symbol::equals(other);
		}

		return false;
	}
};
