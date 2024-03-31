#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <stack>
#include <set>
#include <tuple>

#include "Symbol.h"
#include <unordered_map>

class KnowledgeModel
{
private:
    static std::unordered_map<std::string, int> operatorPrecedence;

    static bool isOperator(const std::string& token) 
    {
        return operatorPrecedence.find(token) != operatorPrecedence.end();
    }

    static bool isOperator(std::string&& token)
    {
        return operatorPrecedence.find(token) != operatorPrecedence.end();
    }

    static bool isHigherPrecedence(
        const std::string& op1,
        const std::string& op2) 
    {
        if (op1 == "(") return false;
        return operatorPrecedence[op1] < operatorPrecedence[op2];
    }

    static std::vector<std::string> tokenizeInfix(const std::string& input)
    {
        std::vector<std::string> tokens;
        std::string token;
        bool isSymbol = false;

        for (char c : input)
        {
            if (c == ' ') continue;

            if (c == '(' || c == ')')
            {
                if (!token.empty())
                {
                    tokens.push_back(token);
                    token.clear();
                }

                tokens.emplace_back(1, c);
                continue;
            }

            if (isalnum(c))
            {
                token += c;
                continue;
            }

            if (!isSymbol)
            {
                if (!token.empty())
                {
                    tokens.push_back(token);
                    token.clear();
                }

                isSymbol = true;
            }

            token += c;

            if (isOperator(token))
            {
                tokens.push_back(token);
                token.clear();
                isSymbol = false;
            }
        }

        if (!token.empty()) tokens.push_back(token);
        return tokens;
    }


    static std::vector<std::string> infixToPostfix(
        const std::vector<std::string>& input)
    {
        std::stack<std::string> operatorStack;
        std::vector<std::string> postfix;

        for (const std::string& token : input) 
        {
            if (token == "(")
            {
                operatorStack.push(token);
            }
            else if (token == ")")
            {
                while (!operatorStack.empty() && operatorStack.top() != "(")
                {
                    postfix.push_back(operatorStack.top());
                    operatorStack.pop();
                }

                if (!operatorStack.empty() && operatorStack.top() == "(")
                {
                    operatorStack.pop();
                }
            }
            else if (!isOperator(token))
            {
                postfix.push_back(token);
            }
            else // Operator
            {
                while (!operatorStack.empty() 
                    && isHigherPrecedence(operatorStack.top(), token))
                {
                    postfix.push_back(operatorStack.top());
                    operatorStack.pop();
                }
                operatorStack.push(token);
            }
        }

        while (!operatorStack.empty())
        {
            postfix.push_back(operatorStack.top());
            operatorStack.pop();
        }

        return postfix;
    }

    const Sentence* postfixToSymbol(const std::vector<std::string>& input)
    {
        if (input.size() == 0) throw std::logic_error("Empty clause!");
        std::vector<const Sentence*> stack; // store component symbols
        Sentence* temp; // placeholder for output symbol
        size_t noOfArg = 0; // store no. of arg. used to construct new symbol
        std::set<const Sentence*>::iterator iter; // placeholder for set insertion
        bool isNotDuplicated; // placeholder for set insertion

        for (size_t i = 0; i < input.size(); i++)
        {
            if (!isOperator(input[i])) // atomic sentence
            {
                temp = new Sentence(input[i]);
                noOfArg = 0;
            }
            else if (input[i] == "~") // unary symbol
            {
                temp = new Not(stack.back());
                noOfArg = 1;
            }
            else if (input[i] == "=>") // binary symbol
            {
                temp = new Imply(
                    stack[stack.size() - 2], 
                    stack[stack.size() - 1]);
                noOfArg = 2;
            }
            else if (input[i] == "<=>") // binary symbol
            {
                temp = new Bicondition(
                    stack[stack.size() - 2], 
                    stack[stack.size() - 1]);
                noOfArg = 2;
            }
            else if (input[i] == "&") // n-nary symbol
            {
                noOfArg = 2;
                while (i + 1 < input.size() && input[i + 1] == "&")
                {
                    i++;
                    noOfArg++;
                }

                temp = new And(std::set<const Sentence*>(
                    std::make_move_iterator(stack.end() - noOfArg),
                    std::make_move_iterator(stack.end())));
            }
            else  // "||" n-nary symbol
            {
                noOfArg = 2;
                while (i + 1 < input.size() && input[i + 1] == "||")
                {
                    i++;
                    noOfArg++;
                }

                temp = new Or(std::set<const Sentence*>(
                    std::make_move_iterator(stack.end() - noOfArg),
                    std::make_move_iterator(stack.end())));
                if (input[i] != "||") throw std::logic_error("Invalid symbol");
            }

            // remove the symbols used to construct the new symbol from the stack
            for (size_t i = 0; i < noOfArg; i++) 
            {
                stack.pop_back();
            }
            
            // push output symbol to the stack
            //  - if there is a similar symbol to temp in set, delete temp
            //  - else insert temp to set
            std::tie(iter, isNotDuplicated) = noOfArg 
                ? compoundSentences.insert(temp)
                : atomicSentences.insert(temp);
            if (!isNotDuplicated) delete temp;
            stack.push_back(*iter);
        }
        
        // return the main symbol
        return stack.back();
    }

    void initialize(const std::vector<std::string>& knowledge,
        const std::string& questionClause)
    {
        for (const std::string& clause : knowledge)
        {
            const Sentence* sentence = postfixToSymbol(infixToPostfix(
                tokenizeInfix(clause)));
            completeSentences.insert(sentence);
        }

        query = postfixToSymbol(infixToPostfix(
            tokenizeInfix(questionClause)));

#ifdef DEVELOP
        std::cout << "\nAtomic Knowledge:" << std::endl;
        for (const Sentence* elem : atomicSentences)
            std::cout << elem->getDescription() << " ";
        
        std::cout << "\n\nCompound Knowledge:" << std::endl;
        for (const Sentence* elem : compoundSentences)
            std::cout << elem->getDescription() << "\n";

        std::cout << "\nComplete Knowledge:" << std::endl;
        for (const Sentence* elem : completeSentences)
            std::cout << elem->getDescription() << "; ";
        
        std::cout << "\n";
#endif  // DEVELOP
    }

public:
    std::set<const Sentence*> atomicSentences;
    std::set<const Sentence*> compoundSentences;
    std::set<const Sentence*> completeSentences;
    const Sentence* query;

    // default constructor
    KnowledgeModel(const std::vector<std::string>& knowledge,
        const std::string& query)
    {
        initialize(knowledge, query);
    }

    // copy constructor
    KnowledgeModel(const KnowledgeModel& other)
    {
        for (const Sentence* sentence : other.atomicSentences)
        {
            atomicSentences.insert(new Sentence(*sentence));
        }

#ifdef DEVELOP
        std::cout << "\nAtomic Knowledge:" << std::endl;
        for (const Sentence* elem : atomicSentences)
            std::cout << elem->getDescription() << " ";
#endif

        for (const Sentence* sentence : other.compoundSentences)
        {
            std::set<const Sentence*>::iterator iter;
            std::set<const Sentence*> set;
            const Sentence* s1;
            const Sentence* s2;
            
            const Not* notSentence;
            const And* andSentence;
            const Or* orSentence;
            const Imply* implySentence;
            const Bicondition* biconSentence;

            if (notSentence = dynamic_cast<const Not*>(sentence))
            {
                iter = compoundSentences.find(notSentence->sentence);
                if (iter != compoundSentences.end())
                {
                    compoundSentences.insert(new Not(*iter));
                    continue;
                }

                iter = atomicSentences.find(notSentence->sentence);
                if (iter != atomicSentences.end())
                {
                    compoundSentences.insert(new Not(*iter));
                    continue;
                }
            }
            else if (andSentence = dynamic_cast<const And*>(sentence))
            {
                for (const Sentence* sentence : andSentence->sentences)
                {
                    iter = compoundSentences.find(sentence);
                    if (iter != compoundSentences.end()) set.insert(*iter);
                    else
                    {
                        iter = atomicSentences.find(sentence);
                        if (iter != atomicSentences.end()) set.insert(*iter);
                        else throw std::logic_error("Symbol not found");
                    }
                }

                compoundSentences.insert(new And(std::move(set)));
            }
            else if (orSentence = dynamic_cast<const Or*>(sentence))
            {
                for (const Sentence* sentence : orSentence->sentences)
                {
                    iter = compoundSentences.find(sentence);
                    if (iter != compoundSentences.end()) set.insert(*iter);
                    else
                    {
                        iter = atomicSentences.find(sentence);
                        if (iter != atomicSentences.end()) set.insert(*iter);
                        else throw std::logic_error("Symbol not found");
                    }
                }

                compoundSentences.insert(new Or(std::move(set)));
            }
            else if (implySentence = dynamic_cast<const Imply*>(sentence))
            {
                iter = compoundSentences.find(implySentence->antecedent);
                if (iter != compoundSentences.end()) s1 = *iter;
                else
                {
                    iter = atomicSentences.find(implySentence->antecedent);
                    if (iter != atomicSentences.end()) s1 = *iter;
                    else throw std::logic_error("Symbol not found");
                }

                iter = compoundSentences.find(implySentence->consequence);
                if (iter != compoundSentences.end()) s2 = *iter;
                else
                {
                    iter = atomicSentences.find(implySentence->consequence);
                    if (iter != atomicSentences.end()) s2 = *iter;
                    else throw std::logic_error("Symbol not found");
                }

                compoundSentences.insert(new Imply(s1,s2));
            }
            else if (biconSentence = dynamic_cast<const Bicondition*>(sentence))
            {
                iter = compoundSentences.find(biconSentence->sentences[0]);
                if (iter != compoundSentences.end()) s1 = *iter;
                else
                {
                    iter = atomicSentences.find(biconSentence->sentences[0]);
                    if (iter != atomicSentences.end()) s1 = *iter;
                    else throw std::logic_error("Symbol not found");
                }

                iter = compoundSentences.find(biconSentence->sentences[1]);
                if (iter != compoundSentences.end()) s2 = *iter;
                else
                {
                    iter = atomicSentences.find(biconSentence->sentences[1]);
                    if (iter != atomicSentences.end()) s2 = *iter;
                    else throw std::logic_error("Symbol not found");
                }

                compoundSentences.insert(new Bicondition(s1, s2));
            }
            else throw std::logic_error("Invalid type of sentence");
        }

#ifdef DEVELOP
        std::cout << "\n\nCompound Knowledge:" << std::endl;
        for (const Sentence* elem : compoundSentences)
            std::cout << elem->getDescription() << "\n";
#endif

        for (const Sentence* sentence : other.completeSentences)
        {
            auto iter = compoundSentences.find(sentence);
            if (iter != compoundSentences.end()) completeSentences.insert(*iter);
            else
            {
                iter = atomicSentences.find(sentence);
                if (iter != atomicSentences.end())completeSentences.insert(*iter);
                else throw std::logic_error("Invalid type of sentence");
            }
        }

#ifdef DEVELOP
        std::cout << "\nComplete Knowledge:" << std::endl;
        for (const Sentence* elem : completeSentences)
            std::cout << elem->getDescription() << "; ";
        std::cout << "\n";
#endif  // DEVELOP

        auto iter = compoundSentences.find(other.query);
        if (iter != compoundSentences.end()) query = *iter;
        else
        {
            iter = atomicSentences.find(other.query);
            if (iter != atomicSentences.end()) query = *iter;
            else throw std::logic_error("Invalid type of sentence");
        }
    }

    ~KnowledgeModel() // destructor
    {
        for (const Sentence* sentence : atomicSentences) delete sentence;
        for (const Sentence* sentence : compoundSentences) delete sentence;

        atomicSentences.clear();
        compoundSentences.clear();
    }
};

std::unordered_map<std::string, int> KnowledgeModel::operatorPrecedence = {
    {"~", 1},
    {"&", 2},
    {"||", 3},
    {"=>", 4},
    {"<=>", 5}
};
