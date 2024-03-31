#pragma once

#include <iostream>
#include <string>

#include "Constants.h"

class Symbol;

class Sentence
{
private:
    const std::string description;
    mutable bool value;

public:
    // no default constructor
    Sentence() = delete;

    // lvalue constructor
    Sentence(const std::string& desc) : description(desc), value(false)
    {
#ifdef TEST
        if (description != "")
            std::cout << "Create sentence \"" << description
                      << "\" with lvalue constructor\n";
#endif  // TEST
    }

    // rvalue constructor
    Sentence(std::string&& desc) : description(std::move(desc)), value(false)
    {
#ifdef TEST
        if (description != "")
            std::cout << "Create sentence \"" << description
                      << "\" with rvalue constructor\n";
#endif  // TEST
    }

    // copy constructor
    Sentence(const Sentence& other) : 
        description(other.description), value(other.value)
    { }

    // virtual destructor
    virtual ~Sentence()
    {
#ifdef TEST
        std::cout << "Delete sentence " << description << "\n";
#endif  // TEST  
    }

    virtual bool operator==(const Sentence& other) const
    {
        return description == other.description;
    }

    bool operator!=(const Sentence& other) const
    {
        return !(description == other.description);
    }

    bool operator<(const Sentence& other) const
    {
        return atomicCount() < other.atomicCount();
    }

    const std::string& getDescription() const
    {
        return description;
    }

    virtual bool evaluate() const
    {
        return value;
    }

    bool setValue(bool val) const 
    {
        value = val;
        return true;
    }

    virtual size_t atomicCount() const
    {
        return 1;
    }

    virtual bool isSymbol() const
    {
        return false;
    }
};

namespace std
{
    template <>
    struct hash<const Sentence*>
    {
        size_t operator()(const Sentence* key) const
        {
            return hash<std::string>()(key->getDescription());
        }
    };

    template <>
    struct equal_to<const Sentence*>
    {
        bool operator()(const Sentence* lhs, const Sentence* rhs) const
        {
            return *lhs == *rhs;
        }
    };

    template <>
    struct less<const Sentence*>
    {
        bool operator()(const Sentence* lhs, const Sentence* rhs) const
        {
            if (lhs->atomicCount() < rhs->atomicCount()) return true;
            else if (lhs->atomicCount() > rhs->atomicCount()) return false;
            else return lhs->getDescription() < rhs->getDescription(); 
        }
    };

    template <>
    struct hash<Sentence*>
    {
        size_t operator()(const Sentence* key) const
        {
            return hash<std::string>()(key->getDescription());
        }
    };

    template <>
    struct equal_to<Sentence*>
    {
        bool operator()(const Sentence* lhs, const Sentence* rhs) const
        {
            return *lhs == *rhs;
        }
    };

    template <>
    struct less<Sentence*>
    {
        bool operator()(const Sentence* lhs, const Sentence* rhs) const
        {
            if (lhs->atomicCount() < rhs->atomicCount()) return true;
            else if (lhs->atomicCount() > rhs->atomicCount()) return false;
            else return lhs->getDescription() < rhs->getDescription();
        }
    };
}