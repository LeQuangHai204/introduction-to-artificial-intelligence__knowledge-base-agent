
## Inference Engine for Propositional Logic
This project implements an inference engine for propositional logic, utilizing the Truth Table (TT) checking, Forward Chaining (FC), and Backward Chaining (BC) algorithms. The engine is designed to determine whether a given query can be entailed from a Horn-form Knowledge Base (KB) using one of the three algorithms. The project is written in C++ with best practices of Object-Oriented Programming (OOP) and design patterns.

## Introduction
The inference engine takes as input a Knowledge Base (KB) in Horn form and a query (q) as a proposition symbol. It then determines whether the query can be entailed from the KB using one of three algorithms: Truth Table checking (TT), Forward Chaining (FC), or Backward Chaining (BC).

## Features
Truth Table Checking (TT): Works with all types of knowledge bases.
Forward Chaining (FC): Works with Horn-form knowledge bases and finds all propositional symbols entailed from KB.
Backward Chaining (BC): Works with Horn-form knowledge bases and finds the propositional symbols required to entail the query.
Command-Line Interface: Supports batch testing through a simple command-line interface.
Object-Oriented Design: Implements OOP best practices and design patterns to ensure modularity, reusability, and maintainability.

## Usage
To run the inference engine, use the following command:
```
> iengine <method> <filename>
```
method: Specifies the algorithm to use (TT for Truth Table checking, FC for Forward Chaining, or BC for Backward Chaining).
filename: The path to the text file containing the problem (knowledge base and query).

## Example
Given the following content in test1.txt:

TELL
```
p2 => p3; p3 => p1; c => e; b & e => f; f & g => h; p1 => d; p1 & p3 => c; a; b; p2;
```
ASK
```
d
```

Run the inference engine using Forward Chaining:
```
> iengine FC test1.txt
```
Expected output:
```
YES: a, b, p2, p3, p1, d
```
Algorithms
Truth Table (TT) Checking
Examines all possible models of the knowledge base to determine if the query is true in all models.
Forward Chaining (FC)
Iteratively infers new facts until the query is found or no more inferences can be made.
Backward Chaining (BC)
Recursively attempts to prove the query by proving all premises of the rules that conclude the query.

## Installation
Clone the repository:
```
git clone https://github.com/yourusername/inference-engine.git
```
Build the project using Makefile:
```
cd inference-engine
make
```
Run the inference engine as described in the Usage section.

## Contributing
Contributions are welcome! Please fork this repository and submit a pull request with your improvements.

