#include <utility>
#include <string>
#include <set>

#include "Constants.h"
#include "Index.h"

int main(int argc, char* argv[])
{
#ifdef TEST
    /*Sentence* pureSen0 = new Sentence("It rains");
    Sentence* pureSen000 = new Sentence("It rains");
    Sentence* pureSen1 = new Sentence("It is cloudy");
    Sentence* and0 = new And({pureSen0, pureSen1});
    Sentence* and1 = new And({pureSen1, pureSen0});
    Sentence* or0 = new Or({pureSen0, pureSen1});
    Sentence* or1 = new Or({pureSen1, pureSen0});
    Sentence* not0 = new Not(pureSen0);
    Sentence* not1 = new Not(pureSen1);

    Sentence* bi0 = new Bicondition({ pureSen0, pureSen1 });
    Sentence* bi1 = new Bicondition({ pureSen1, pureSen0 });
    
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
    std::cout << "Compare: " << ((*bi0) == (*bi1)) << "\n";
    std::cout << "Compare: " << ((*pureSen0) == (*pureSen000)) << "\n";
    return 0;*/
#endif  // TEST


    auto [knowledge, query] = FileReader::readFile(argc, argv);
    KnowledgeModel knowledgeModel(knowledge, query);

#ifdef TEST
    std::cout << "---------------------------------------\n";
    KnowledgeModel copy(knowledgeModel);
#endif // TEST

    TruthTable truthTable(knowledgeModel);
    std::cout << "\nQuery result: \n";
    bool result = truthTable.execute();
    std::cout << "\nIs query true in all world?  " << (result ? "YES" : "NO") << "\n";
}


