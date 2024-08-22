#pragma once

#include "Imply.h"
#include "KnowledgeModel.h"

class ForwardChaining
{
private:
    const KnowledgeModel* knowledgeModel;
    std::vector<const Sentence*> inferredSentences;

    void checkPremise(const Sentence* currentSentence)
    {
        for (const Sentence* sentence : knowledgeModel->compoundSentences)
        {
            std::cout << sentence->getDescription() << " is considered\n";
            const Imply* placeholder = dynamic_cast<const Imply*>(sentence);

            if (!placeholder)
            {
                std::cout << sentence->getDescription() << " is not an Imply\n";
                continue;
            }

            if (placeholder->consequence->isSymbol())
            {
                throw std::logic_error("Not Horn clause");
            }

            if (!placeholder->isInAntecedent(currentSentence))
            {
                std::cout << sentence->getDescription() << " does not contain " << currentSentence->getDescription()
                          << "\n";
                continue;
            }

            if (--placeholder->premiseCount)
            {
                std::cout << placeholder->getDescription() << " premise is set to " << placeholder->premiseCount
                          << "\n";
                continue;
            }

            std::cout << placeholder->consequence->getDescription() << " to true\n";
            placeholder->consequence->setValue(true);
            inferredSentences.push_back(placeholder->consequence);
            checkPremise(placeholder->consequence);
        }
    }
public:
    ForwardChaining(const KnowledgeModel& knowledgeModel) : knowledgeModel(&knowledgeModel)
    {
        for (const Sentence* sentence : knowledgeModel.compoundSentences)
        {
            if (!(dynamic_cast<const Imply*>(sentence) || dynamic_cast<const And*>(sentence)))
            {
                throw std::domain_error("Model only accept Horn knowledge");
            }
        }
    }

    bool execute()
    {
        for (const Sentence* sentence : knowledgeModel->atomicSentences)
        {
            sentence->setValue(false);

            std::cout << sentence->getDescription() << " to false\n";
        }

        for (const Sentence* sentence : knowledgeModel->completeSentences)
        {
            if (!sentence->isSymbol())
            {
                std::cout << sentence->getDescription() << " to true\n";
                sentence->setValue(true);
                checkPremise(sentence);
            }
        }

        std::cout << "\n\nInferred list:  ";
        for (const Sentence* sentence : inferredSentences)
        {
            std::cout << sentence->getDescription() << " ; ";
        }

        return std::find(inferredSentences.begin(), inferredSentences.end(), knowledgeModel->query)
            != inferredSentences.end();
    }
};