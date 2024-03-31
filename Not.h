#pragma once

#include "Symbol.h"
#include "Constants.h"

class Not : public Symbol
{
public:
	const Sentence* sentence;

	Not(const Sentence* sentence) : Symbol(sentence->isSymbol() 
		? "~(" + sentence->getDescription() + ")" 
		: "~" + sentence->getDescription()), 
		sentence(sentence)
	{
		if (!sentence)
		{
			throw std::invalid_argument("Null sentence in Not constructor");
		}
	}

	~Not()
	{
#ifdef TEST
		std::cout << "Not deleting: " << getDescription() << "\n";
#endif
	}

	bool evaluate() const override
	{
		return !sentence->evaluate();
	}

	size_t atomicCount() const override
	{
		return sentence->atomicCount();
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
