#include <utility>
#include <string>
#include <set>

#include "Constants.h"
#include "Index.h"

int main(int argc, char* argv[])
{
    auto [knowledge, query] = FileReader::readFile(argc, argv);
    if (query == "") return 1;
    KnowledgeModel knowledgeModel(knowledge, query);

    try {

        if (argv[2] == "TT")
        {
            TruthTable truthTable(knowledgeModel);
            std::cout << "\nQuery result: \n";
            bool result = truthTable.execute();
            std::cout << "\nIs query true in all world?  "
                << (result ? ("YES: " + std::to_string(truthTable.getCount())) : "NO") << "\n";
        }
        else if (argv[2] == "FC")
        {
            ForwardChaining fwc(knowledgeModel);
            bool result = fwc.execute();
            std::cout << "\nIs query true?  " << (result ? "YES" : "NO") << "\n";
        }
        else if (argv[2] == "BC")
        {
            BackwardChaining bwc(knowledgeModel);
            bool result = bwc.execute();
            std::cout << "\nIs query true?  " << (result ? "YES" : "NO") << "\n";
        }
        else std::cout << "Invalid logic model (TT/FC/BC)\n";

    }
    catch (...)
    {
        std::cout << "Caught an exception!" << std::endl;
    }
}


