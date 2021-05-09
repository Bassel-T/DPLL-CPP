# DPLL-CPP

## Usage
`make run filename=[KB File]`

## The Algorithm and Goal

DPLL, or the "Davis-Putnam-Logemann-Loveland" algorithm finds a correct model for a given knowledge base (in Conjunctive Normal Form), provided one exists. In this project, I've programmed the algorithm in C++, including the Unit-Clause Heuristic, which finds an unresolved clause with only one symbol and evalutates it, thus pruning unnecessary sections of the model tree before running them.

The ultimate goal of this project was to create a Sudooku board solver, as inefficient as it may be. The CNF was generated using `sudoku.py`. There are duplicates in the model because of how I programmed the KB generator. However, the algorithm removes duplicates before running. In addition, I skipped a few rules (at least one element in a row/column/region must contain digits 1-9).

The sudoku board has 81 tiles, each with 9 possible values, leading to 729 symbols in the KB, and about 2.8 * 10^218 possible models, including invalid ones (ones where two ones appear adjacent to one another).

Due to the number of items, a recursive DPLL is unacceptable, as it would undoubtedly lead to a Stack Overflow error. Thus, I remade the algorithm to work iteratively, minimizing the stack trace. 