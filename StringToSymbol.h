#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <stack>
#include <unordered_set>

#include "Symbol.h"
#include <unordered_map>

class StringToSymbol
{
public:
    static std::unordered_set<Sentence> atomicSentences;
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

    static std::vector<std::string> tokenize(const std::string& input)
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

    static Sentence* postfixToSymbol(const std::vector<std::string>& input)
    {
        if (input.size() == 0) throw std::logic_error("Empty clause!");
        std::vector<Sentence*> stack;
        Sentence* temp;
        size_t noOfArg = 0;

        for (size_t i = 0; i < input.size(); i++)
        {
            if (!isOperator(input[i]))
            {
                stack.push_back(&const_cast<Sentence&>(
                    *atomicSentences.emplace(input[i]).first));
                continue;
            }

            if (input[i] == "~")
            {
                temp = new Not(stack.back());
                noOfArg = 1;
            }
            else if (input[i] == "=>")
            {
                temp = new Imply(
                    stack[stack.size() - 1], 
                    stack[stack.size() - 2]);
                noOfArg = 2;
            }
            else if (input[i] == "<=>")
            {
                temp = new Bicondition(
                    stack[stack.size() - 1], 
                    stack[stack.size() - 2]);
                noOfArg = 2;
            }
            else if (input[i] == "&")
            {
                temp = new And({
                    stack[stack.size() - 1],
                    stack[stack.size() - 2] });
                noOfArg = 2;
            }
            else  // "||"
            {
                temp = new Or({
                    stack[stack.size() - 1],
                    stack[stack.size() - 2]});
                noOfArg = 2;
            }

            for (size_t i = 0; i < noOfArg; i++)
            {
                stack.pop_back();
            }

            stack.push_back(temp);
        }

        return stack.back();
    }

public:
    static Sentence* convert(const std::string& infix)
	{
        return postfixToSymbol(infixToPostfix(tokenize(infix)));
	}

    static void test(const std::string& input)
    {
        //Sentence* postfix = postfixToSymbol(infixToPostfix(tokenize(input)));
        //std::cout << "Postfix Description: " << postfix->getDescription() << std::endl;
        //delete postfix; // Don't forget to free the memory

        ////for (const std::string& token : postfix) std::cout << token << "";
        //std::cout << std::endl;
    }
};

std::unordered_map<std::string, int> StringToSymbol::operatorPrecedence = {
    {"~", 1},
    {"&", 2},
    {"||", 3},
    {"=>", 4},
    {"<=>", 5}
};

std::unordered_set<Sentence> StringToSymbol::atomicSentences;