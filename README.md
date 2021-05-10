# DPLL-CPP

## Usage
`make run filename=[KB File]`

## The Algorithm and Goal

DPLL, or the "Davis-Putnam-Logemann-Loveland" algorithm finds a correct model for a given knowledge base (in Conjunctive Normal Form), provided one exists. In this project, I've programmed the algorithm in C++, including the Unit-Clause Heuristic, which finds an unresolved clause with only one symbol and evalutates it, thus pruning unnecessary sections of the model tree before running them.

The ultimate goal of this project was to create a Sudooku board solver, as inefficient as it may be. The CNF was generated using `sudoku.py`. There are duplicates in the model because of how I programmed the KB generator. However, the algorithm removes duplicates before running. In addition, I skipped a few rules (at least one element in a row/column/region must contain digits 1-9).

The sudoku board has 81 tiles, each with 9 possible values, leading to 729 symbols in the KB, and about 2.8 * 10^218 possible models, including invalid ones (ones where two ones appear adjacent to one another).

Due to the number of items, a recursive DPLL is unacceptable, as it would undoubtedly lead to a Stack Overflow error. Thus, I remade the algorithm to work iteratively, minimizing the stack trace.

## Sudoku Data

Sudoku wouldn't run as fast had I changed the order of the CNF. This is because the Unit Heuristic works best with the "no two tiles in these scenarios can have the same number" or the "no tiles can have two numbers." By searching through the shortest clauses first, I can find accurate models much faster than if they were at the end of the KB.

Tiles are listed in the following way: `[row][column][digit]` starting from the top-left. Rows and columns are denoted as a letter from A to I. The first (however many) rows in the knowledge base are the tiles given in the puzzle.

Since the output is the model, I wrote a python program to display what the grid looks like for sudoku. To run this, redirect the output of `make run` into a file called `output.txt`, then run `python3 formatter.py`.