#pragma once

#include "And.h"
#include "Imply.h"
#include "KnowledgeModel.h"

// ------------------------------- NOW IT WORKS !!!!!!!! -----------------
// ------------------------------- RUN AGAIN PLEASE ----------------------
// ------------------------------- BEGGINGGGGGGGGGGGGGGGG ----------------
// ------------------------------- BEGGINGGGGGGGGGGGGGGGG ----------------
// ------------------------------- BEGGINGGGGGGGGGGGGGGGG ----------------
// ------------------------------- BEGGINGGGGGGGGGGGGGGGG ----------------
// ------------------------------- BEGGINGGGGGGGGGGGGGGGG ----------------

class BackwardChaining
{
private:
    const KnowledgeModel* knowledgeModel;
    std::stack<const Sentence*> goals;
    std::stack<const Sentence*> solution;

    void printGoals()
    {
        std::cout << "Stack contain: ";
        std::stack<const Sentence*> tempStack;
        while (!goals.empty())
        {
            const Sentence* element = goals.top();
            std::cout << element->getDescription() << " ";
            tempStack.push(element);
            goals.pop();
        }
        while (!tempStack.empty())
        {
            auto element = tempStack.top();
            goals.push(element);
            tempStack.pop();
        }
        std::cout << "\n";
    }

    bool trackClause(const Sentence* query)
    {
        if (goals.empty()) return false;
        goals.pop();
        bool foundNextGoal = false;

        for (const Sentence* sentence : knowledgeModel->compoundSentences)
        {
            std::cout << "Sentence is: " << sentence->getDescription() << "\n";
            const Imply* placeholder = dynamic_cast<const Imply*>(sentence);

            // placeholder is not imply proposition
            if (!placeholder)
            {
                std::cout << sentence->getDescription() << " is not Imply clause\n";
                continue;
            }

            // placeholder does not imply query
            if (placeholder->consequence != query)
            {
                std::cout << placeholder->getDescription() << " cannot resolve to " << query->getDescription() << "\n";
                continue;
            }

            // antecedence is atomic sentence
            if (!placeholder->antecedent->isSymbol())
            {
                // the antecedent is not in given knowledge
                if (!knowledgeModel->completeSentences.contains(placeholder->antecedent))
                {
                    std::cout << placeholder->getDescription() << " - " << placeholder->antecedent->getDescription()
                              << " is added as new goal\n";
                    goals.push(placeholder->antecedent);

                    solution.push(placeholder);

                    printGoals();
                    return true;
                }

                solution.push(placeholder);

                printGoals();
                return true;
            }

            // the antecedent is conjuction
            const And* hornAntecedent = dynamic_cast<const And*>(placeholder->antecedent);
            if (hornAntecedent == nullptr) throw std::logic_error("Not Horn clause");

            std::cout << placeholder->antecedent->getDescription() << " is conjuction\n";
            bool temp = true;
            for (const Sentence* s : hornAntecedent->sentences)
            {
                if (!s->getValue())
                {
                    temp = false;
                    std::cout << s->getDescription() << " is added as new goal\n";
                    goals.push(s);
                    foundNextGoal = true;
                }
            }

            if (temp == true)
            {
                solution.push(placeholder);
                return true;
            }

            std::cout << placeholder->getDescription() << " is added to solution\n";
            solution.push(placeholder);
            break;
        }

        if (!solution.empty() && foundNextGoal == false)
        {
            std::cout << solution.top()->getDescription() << "Is popped\n";
            solution.pop();
            return false;
        }
        return true;
    }
public:
    BackwardChaining(const KnowledgeModel& knowledgeModel) : knowledgeModel(&knowledgeModel)
    {
        for (const Sentence* sentence : knowledgeModel.compoundSentences)
            if (!(dynamic_cast<const Imply*>(sentence) || dynamic_cast<const And*>(sentence)))
                throw std::domain_error("Model only accept Horn knowledge");
    }

    bool execute()
    {
        for (const Sentence* sentence : knowledgeModel->atomicSentences)
        {
            sentence->setValue(false);
        }

        for (const Sentence* sentence : knowledgeModel->completeSentences)
        {
            if (!sentence->isSymbol())
            {
                sentence->setValue(true);
            }
        }

        goals.push(knowledgeModel->query);
        while (!goals.empty())
        {
            printGoals();
            if (!trackClause(goals.top()))
            {
                return false;
            }
        }

        std::cout << "\nSequence:\n";
        while (!solution.empty())
        {
            std::cout << solution.top()->getDescription() << "\n";
            solution.pop();
        }

        return true;
    }
};