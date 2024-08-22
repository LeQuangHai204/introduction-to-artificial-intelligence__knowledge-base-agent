#pragma once

#include "Sentence.h"

class Symbol : public Sentence
{
protected:
    Symbol(std::string&& description) : Sentence(std::move(description)) { }

    virtual ~Symbol() = default;

    virtual bool isSymbol() const override { return true; }

    friend struct std::hash<Symbol*>;
};
