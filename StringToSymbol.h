#pragma once

#include<string>
#include<iostream>
#include<stack>

#include "Symbol.h"

class StringToSymbol
{
public:
	static Symbol* convert(std::string input)
	{
        std::string inputString = "(a<=>(cds=>~dss))&b&(b=>a)";
        // And(Bicondition(Imply(c,Not(d)),a),b,Imply(b,a))

        std::stack<size_t> parenthesisOpen;
        std::stack<size_t> parenthesisClose;
        
        size_t currentIndex = 0;
        while (currentIndex < inputString.size())
        {
            if (inputString[currentIndex] == '(')
            {
                parenthesisOpen.push(currentIndex);
            }
            else if (inputString[currentIndex] == ')')
            {
                parenthesisOpen.pop(); 
                if (--parenthesisCount == 0)
                {
                    Symbol* tempSymbol = convert(input.substr(,1));
                }
            }

            currentIndex++;
        }
	}
};