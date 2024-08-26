#pragma once

#include "Constants.h"
#include "Symbol.h"

#include <numeric>
#include <set>

class And : public Symbol
{
public:
    std::set<const Sentence*> sentences;

    And(std::set<const Sentence*>&& sentences) :
        Symbol(
            [](std::set<const Sentence*>* set) -> std::string
            {
                std::string result = std::accumulate(
                    set->begin(),
                    set->end(),
                    std::string(),
                    [](std::string&& result, const Sentence* sentence)
                    {
                        if (!sentence) throw std::domain_error("Symbol contains NULL");
                        if (sentence->isSymbol()) result += "(";
                        result += sentence->getDescription();
                        if (sentence->isSymbol()) result += ")";
                        result += "&";
                        return std::move(result);
                    });
                return result.substr(0, result.size() - 1);
            }(&sentences)),
        sentences(std::move(sentences))
    {
        if (this->sentences.size() < 2) throw std::domain_error("And clause requires at least 2 sentences");
    }

    ~And()
    {
#ifdef TEST
        std::cout << "And deleting: " << getDescription() << "\n";
#endif
    }

    bool evaluate() const override
    {
        for (const Sentence* sentence : sentences)
        {
            if (!sentence->evaluate())
            {
                return false;
            }
        }

        return true;
    }

    size_t atomicCount() const override
    {
        size_t result = 0;
        for (const Sentence* sentence : sentences)
        {
            result += sentence->atomicCount();
        }

        return result;
    }

    bool contains(const Sentence* sentence) const { return sentences.contains(sentence); }

    bool operator==(const Sentence& other) const override
    {
        const And* andSymbol = dynamic_cast<const And*>(&other);
        return andSymbol && operator==(*andSymbol);
    }

    bool operator==(const And& other) const { return sentences == other.sentences; }
};
