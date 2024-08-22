You need to implement an inference engine for propositional logic in software based on the Truth Table (TT) checking, and Backward Chaining (BC) and Forward Chaining (FC) algorithms. Your inference engine will take as arguments a Horn-form Knowledge Base KB and a query q which is a proposition symbol and determine whether q can be entailed from KB. You will also need to write a report about how your program works with different knowledge bases and queries.

The Truth Table Checking (TT) algorithm works with all types of knowledge bases. The Forward Chaining (FC) and Backward Chaining (BC) algorithms work with Horn-form knowledge bases. Given a knowledge base KB in Horn form (with TELL) and a query q which is a proposition symbol (with ASK), your program needs to answer whether or not q is entailed from KB using one of the three algorithms TT, or FC, or BC.

The problems are stored in simple text files consisting of both the knowledge base and the query: 
- The knowledge base follows the keyword TELL and consists of Horn clauses separated by
semicolons.
- The query follows the keyword ASK and consists of a proposition symbol.
For example, the following could be the content of one of the test files (test1.txt):
TELL
p2=> p3; p3 => p1; c => e; b&e => f; f&g => h; p1=>d; p1&p3 => c; a; b; p2;
ASK
d

Your program needs to operate in a simple command-line form to support batch testing. This can be
accomplished with a simple DOS .bat (batch) file if needed. Below is an example of how your program will be
run: 
> iengine method filename
where iengine is your .exe file or a .bat (batch) file that calls your program with the parameters, method
can be either TT (for Truth Table checking), or FC (for Forward Chaining), or BC (for Backward Chaining) to
specify the algorithm, and filename is for the text file consisting of the problem.
For instance:
> iengine FC test1.txt
Standard output is an answer of the form YES or NO, depending on whether the ASK(ed) query q follows
from the TELL(ed) knowledge base KB. When the method is TT and the answer is YES, it should be
followed by a colon (:) and the number of models of KB. When the method is FC or BC and the answer is
YES, it should be followed by a colon (:) and the list of propositional symbols entailed from KB that has been
found during the execution of the specified algorithm.
For example, running iengine with method TT on the example test1.txt file should produce the
following output:
> YES: 3
On the other hand, when I run my implementation of iengine with method FC on the example test1.txt
it produces the following output:
> YES: a, b, p2, p3, p1, d
Note that your implementation might produce a different output and it would still be correct, depending on the
order of the sentences in the knowledge base. I’ll check that carefully to ensure that you won’t be
disadvantaged if your results don’t look exactly the same as my results.
And running iengine with method BC on the example test file above should produce the following output:
> YES: p2, p3, p1, d
