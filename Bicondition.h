#pragma once

#include "Symbol.h"
#include "Constants.h"

class Bicondition : public Symbol
{
public:
	const Sentence* const sentences[2];

	Bicondition(const Sentence* s1, const Sentence* s2) : Symbol((
		   (s1->getDescription() < s2->getDescription() ? s1 : s2)->isSymbol() ? "(" : "")
		+  (s1->getDescription() < s2->getDescription() ? s1 : s2)->getDescription()
		+ ((s1->getDescription() < s2->getDescription() ? s1 : s2)->isSymbol() ? ")" : "")
		+ "<=>"
		+ ((s1->getDescription() < s2->getDescription() ? s2 : s1)->isSymbol() ? "(" : "")
		+  (s1->getDescription() < s2->getDescription() ? s2 : s1)->getDescription()
		+ ((s1->getDescription() < s2->getDescription() ? s2 : s1)->isSymbol() ? ")" : "")),
		sentences { s1->getDescription() < s2->getDescription() ? s1 : s2,  s1->getDescription() < s2->getDescription() ? s2 : s1 }
	{
		if (!s1 || !s2)
		{
			throw std::invalid_argument("Null sentence in Bicondition constructor");
		}
	}

	~Bicondition()
	{
#ifdef TEST
		std::cout << "Bicon deleting: " << getDescription() << "\n";
#endif
	}

	bool evaluate() const override
	{
		return sentences[0]->evaluate() == sentences[1]->evaluate();
	}

	size_t atomicCount() const override
	{
		return sentences[0]->atomicCount() + sentences[1]->atomicCount();
	}

	bool operator==(const Sentence& other) const override
	{
		const Bicondition* biconditionSymbol = dynamic_cast<const Bicondition*>(&other);
		return biconditionSymbol && operator==(*biconditionSymbol);
	}

	bool operator==(const Bicondition& other) const
	{
		return (sentences[0] == other.sentences[0]
			&& sentences[1] == other.sentences[1]);
	}
};
