#include <utility>
#include <string>
#include <set>

#include "Constants.h"
#include "Index.h"

int main(int argc, char* argv[])
{
#ifdef TEST
   /* Sentence* pureSen0 = new Sentence("It rains");
    Sentence* pureSen000 = new Sentence("It rains");
    Sentence* pureSen1 = new Sentence("It is cloudy");
    Sentence* and0 = new And({pureSen0, pureSen1});
    Sentence* and1 = new And({pureSen1, pureSen0});
    Sentence* or0 = new Or({pureSen0, pureSen1});
    Sentence* or1 = new Or({pureSen1, pureSen0});
    Sentence* not0 = new Not(pureSen0);
    Sentence* not1 = new Not(pureSen1);

    std::cout << "\nCompare: " << (pureSen0 < pureSen1) << "\n";
    std::cout << "Compare: " << (and0 < not0) << "\n";

    Sentence* bi0 = new Bicondition( pureSen0, pureSen1 );
    Sentence* bi1 = new Bicondition( pureSen1, pureSen0 );
    
    std::set<Sentence*> set;
    std::cout << set.insert(bi0).second << "\n";
    std::cout << set.insert(bi1).second << "\n";
    std::cout << set.insert(and0).second << "\n";
    std::cout << set.insert(and1).second << "\n";
    for (Sentence* sentence : set)
    {
        std::cout << sentence->getDescription() << "\n";
    }

    auto it = set.find(and0);
    if (it != set.end())
    {
        Sentence* foundObject = *it;
        std::cout << "Found Object: " << foundObject->getDescription() << "\n";
    }
    
    std::cout << "Contains: " << set.contains(bi1) << "\n";
    std::cout << "Compare: " << ((*bi0) == (*bi1)) << "\n";
    std::cout << "Compare: " << ((*pureSen0) == (*pureSen000)) << "\n";
    return 0; */
#endif  // TEST
    
    
    auto [knowledge, query] = FileReader::readFile(argc, argv);
    if (query == "") return 1;
    KnowledgeModel knowledgeModel(knowledge, query);

#ifdef TEST
    std::cout << "---------------------------------------\n";
    KnowledgeModel copy(knowledgeModel);
#endif // TEST

    try {

#ifdef TRUTH_TABLE
        TruthTable truthTable(knowledgeModel);
        std::cout << "\nQuery result: \n";
        bool result = truthTable.execute();
        std::cout << "\nIs query true in all world?  "
            << (result ? ("YES: " + std::to_string(truthTable.getCount())) : "NO") << "\n";
#endif // TRUTH_TABLE

#ifdef FORWARD_CHAINING
        ForwardChaining fwc(knowledgeModel);
        bool result = fwc.execute();
        std::cout << "\nIs query true?  " << (result ? "YES" : "NO") << "\n";
#endif // FORWARD_CHAINING

#ifdef BACKWARD_CHAINING
        BackwardChaining bwc(knowledgeModel);
        bool result = bwc.execute();
        std::cout << "\nIs query true?  " << (result ? "YES" : "NO") << "\n";
#endif // FORWARD_CHAINING

    }
    catch (...)
    {
        std::cout << "Caught an exception!" << std::endl;
    }
}


