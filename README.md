# DPLL-CPP

DPLL, or the "Davis-Putnam-Logemann-Loveland" algorithm finds a correct model for a given knowledge base (in Conjunctive Normal Form), provided one exists. In this project, I've programmed the algorithm in C++, including the Unit-Clause Heuristic, which finds a clause with only one symbol and evalutates it, thus pruning unnecessary sections of the model tree before running them.

The ultimate goal of this project was to create a Sudooku board solver, as inefficient as it may be. The CNF was generated using `sudoku.py`, though it's entirely possible I missed a few rules, especially with a KB of over 11,000 entries.

A sudoku board has 81 tiles, each with 9 possible values, leading us to 729 symbols, and 10^218 possible models. Naturally, it is impossible to check them all, though that will not stop me from trying to run the algorithm regardless. After trying to run it on a server, I had to terminate the code prior to completion. After checking the CPU usage, the program was at 99.9% of the CPU (when checking `ps aux`). It occurred to me that the recursion in this will eventually cause a Stack Overflow error before it actually finds a model. In addition, I realized some of the clauses appear in the KB twice.