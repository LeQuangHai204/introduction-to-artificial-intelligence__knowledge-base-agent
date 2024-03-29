#pragma once

#include "Symbol.h"
#include "Constants.h"

class Bicondition : public Symbol
{
private:
	Sentence* sentences[2];

public:
	Bicondition(Sentence* s1, Sentence* s2) :
		sentences { s1, s2 } 
	{
		if (!s1 || !s2)
		{
			throw std::invalid_argument("Null sentence in Bicondition constructor");
		}
	}

	std::string getDescription() const override
	{
		return (sentences[0]->isSymbol() ? "(" : "")
			+ sentences[0]->getDescription()
			+ (sentences[0]->isSymbol() ? ")" : "")
			+ " <=> "
			+ (sentences[1]->isSymbol() ? "(" : "")
			+ sentences[1]->getDescription()
			+ (sentences[1]->isSymbol() ? ")" : "");
	}

	bool getValue() const override
	{
		return sentences[0]->getValue() == sentences[1]->getValue();
	}
};
