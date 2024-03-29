#pragma once

#include "Sentence.h"

class Symbol : public Sentence
{
protected:
	Symbol() = default;
	virtual ~Symbol() = default;
	virtual bool isSymbol() const override { return true; }
	virtual std::string getDescription() const = 0;

	friend struct std::hash<Symbol>;
};

namespace std
{
	template<> struct hash<Symbol>
	{
		size_t operator()(const Symbol& key) const
		{
			// Define a proper hash function for Symbol objects
			// For example, you can combine the hash values of its member variables
			return hash<std::string>()(key.getDescription());
		}
	};
}