#pragma once

#include "Symbol.h"
#include "Constants.h"

class Imply : public Symbol
{
public:
	const Sentence* const antecedent;
	const Sentence* const consequence;
	mutable size_t premiseCount;

	Imply(const Sentence* antecedent, const Sentence* consequence) : 
		Symbol((
			antecedent->isSymbol() ? "(" : "")
			+ antecedent->getDescription()
			+ (antecedent->isSymbol() ? ")" : "")
			+ "=>"
			+ (consequence->isSymbol() ? "(" : "")
			+ consequence->getDescription()
			+ (consequence->isSymbol() ? ")" : "")),
		antecedent(antecedent), 
		consequence(consequence), 
		premiseCount(antecedent->atomicCount())
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

	// for forward chaining
	bool isInAntecedent(const Sentence* sentence) const 
	{
		if (!sentence) throw std::logic_error("Null cannot be symbol component");
		if (!antecedent->isSymbol()) return *antecedent == *sentence;
		const And* andSentence = dynamic_cast<const And*>(antecedent);
		if (!andSentence) throw std::logic_error("This method only apply for Horn form");
		return andSentence->contains(sentence);
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
