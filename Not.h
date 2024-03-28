#pragma once

#include "Symbol.h"
#include "Constants.h"

class Not : public Symbol
{
public:
	Not(Sentence* sentence) : Symbol(std::vector{ sentence }) { }

	std::string getDescription() const override
	{
		if (sentences[0]->isSymbol()) return
			"~ (" + sentences[0]->getDescription() + ")";
		return "~ " + sentences[0]->getDescription();
	}

	bool getValue() const override
	{
		return !sentences[0]->getValue();
	}

	bool equals(const Symbol& other) const override
	{
		// Check if the other Symbol is of type And
		const Not* notSymbol = dynamic_cast<const Not*>(&other);
		return notSymbol && sentences[0] == notSymbol->sentences[0];
	}
};
