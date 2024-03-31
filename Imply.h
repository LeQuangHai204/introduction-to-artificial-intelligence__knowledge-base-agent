#pragma once

#include "Symbol.h"
#include "Constants.h"

class Imply : public Symbol
{
public:
	const Sentence* const antecedent;
	const Sentence* const consequence;

	Imply(const Sentence* antecedent, const Sentence* consequence) : Symbol((
		antecedent->isSymbol() ? "(" : "")
		+ antecedent->getDescription()
		+ (antecedent->isSymbol() ? ")" : "")
		+ "=>"
		+ (consequence->isSymbol() ? "(" : "")
		+ consequence->getDescription()
		+ (consequence->isSymbol() ? ")" : "")),
		antecedent(antecedent), consequence(consequence)
	{
		if (!antecedent || !consequence)
		{
			throw std::invalid_argument("Null sentence in Imply constructor");
		}
	}

	~Imply()
	{
#ifdef TEST
		std::cout << "Imply deleting: " << getDescription() << "\n";
#endif
	}

	bool evaluate() const override
	{
		return !antecedent->evaluate() || consequence->evaluate();
	}

	size_t atomicCount() const override
	{
		return antecedent->atomicCount() + consequence->atomicCount();
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
