#pragma once

#include <string>
#include <utility>

#include "Sentence.h"

class Symbol : public Sentence
{
protected:
	std::vector<Sentence*> sentences;

	Symbol(std::vector<Sentence*>&& sentences) : Sentence(""), 
		sentences(std::move(sentences))
	{ }

	virtual ~Symbol()
	{ };

	bool isSymbol() const override
	{
		return true;
	}

	virtual bool equals(const Symbol& other) const
	{
		if (sentences.size() != other.sentences.size()) 
		{
			return false;
		}

		for (size_t i = 0; i < sentences.size(); ++i)
		{
			if (*sentences[i] != *other.sentences[i])
			{
				return false;
			}
		}

		return true;
	}

public:
	virtual bool operator==(const Sentence& other) const override
	{
		if (const Symbol* otherSymbol = dynamic_cast<const Symbol*>(&other))
		{
			return equals(*otherSymbol);
		}

		return false;
	}
};

namespace std
{
	template<>
	struct hash<Symbol>
	{
		size_t operator()(const Symbol& key) const
		{
			// Define a proper hash function for Symbol objects
			// For example, you can combine the hash values of its member variables
			return hash<std::string>()(key.getDescription());
		}
	};
}