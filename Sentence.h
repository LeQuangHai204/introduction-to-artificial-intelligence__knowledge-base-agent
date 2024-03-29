#pragma once

#include <string>
#include <iostream>

#include "Constants.h"

class Symbol;

class Sentence
{
private:
	const std::string description;
	bool value;

protected:
	Sentence() : description(""), value(true)
	{ }

public:
	// lvalue constructor
	Sentence(const std::string& desc) : description(desc), value(true)
	{
#ifdef TEST
		if(description != "") std::cout << "Create sentence \"" << description << "\" with lvalue constructor\n";
#endif // TEST
	}
		
	// rvalue constructor
	Sentence(std::string&& desc) : description(std::move(desc)), value(true)
	{
#ifdef TEST
		if (description != "") std::cout << "Create sentence \"" << description << "\" with rvalue constructor\n";
#endif // TEST
	}

	virtual ~Sentence()
	{ }

	virtual bool operator==(const Sentence& other) const
	{
		return description == other.description;
	}

	bool operator!=(const Sentence& other) const
	{
		return !(description == other.description);
	}

	virtual std::string getDescription() const
	{
		return description;
	}

	virtual bool getValue() const
	{
		return value;
	}

	bool setValue(bool val)
	{
		value = val;
	}

	virtual bool isSymbol() const
	{
		return false;
	}
};

namespace std
{
	template<> struct hash<Sentence>
	{
		size_t operator()(const Sentence& key) const
		{
			// Define a proper hash function for Symbol objects
			// For example, you can combine the hash values of its member variables
			return hash<std::string>()(key.getDescription());
		}
	};
}