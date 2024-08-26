#include "Constants.h"
#include "Index.h"

#include <iostream>
#include <set>
#include <string>
#include <utility>

enum LogicModel
{
    TT,
    FC,
    BC,
    INVALID
};

LogicModel getLogicModel(const std::string& model)
{
    if (model == "TT") return TT;
    if (model == "FC") return FC;
    if (model == "BC") return BC;
    return INVALID;
}

int main(int argc, char* argv[])
{
    auto [knowledge, query] = FileReader::readFile(argc, argv);
    if (query.empty()) return 1;

    KnowledgeModel knowledgeModel(knowledge, query);

    try
    {
        LogicModel model = getLogicModel(argv[2]);

        switch (model)
        {
            case TT:
            {
                TruthTable truthTable(knowledgeModel);
                std::cout << "\nQuery result: \n";
                bool result = truthTable.execute();
                std::cout << "\nIs query true in all worlds? "
                          << (result ? ("YES: " + std::to_string(truthTable.getCount())) : "NO") << "\n";
                break;
            }
            case FC:
            {
                ForwardChaining fwc(knowledgeModel);
                bool result = fwc.execute();
                std::cout << "\nIs query true? " << (result ? "YES" : "NO") << "\n";
                break;
            }
            case BC:
            {
                BackwardChaining bwc(knowledgeModel);
                bool result = bwc.execute();
                std::cout << "\nIs query true? " << (result ? "YES" : "NO") << "\n";
                break;
            }
            default:
            {
                std::cout << "Invalid logic model (TT/FC/BC)\n";
                break;
            }
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Caught an exception: " << e.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "Caught an unknown exception!" << std::endl;
    }

    return 0;
}
