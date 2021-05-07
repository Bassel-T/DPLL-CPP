#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <unordered_map>

#include "parser.h"

using namespace std;

/*
False = 0
Unknown = 0.5
True = 1
*/

int IndexOf(vector<string> haystack, string needle) {
    for (int i = 0, s = haystack.size(); i < s; i++) {
        if (strcmp(haystack.at(i).c_str(), needle.c_str()) == 0) {
            // There's a match here
            return i;
        }
    }

    // There's no match anywhere
    return -1;
}

// Find all the symbols in the knowledge base
vector<string> UniqueSymbols(vector<CNF*> kb) {
    vector<string> toReturn;

    for (CNF* line : kb) {
        
        // Get the clause as a string
        stringstream clause(line->toString());
        string symbol;
        while (clause >> symbol) {
            if (symbol.at(0) == '-') {
                // Current symbol is a negation
                if (IndexOf(toReturn, symbol.substr(1)) == -1) {
                    toReturn.push_back(symbol.substr(1));
                }
            } else {
                // Current symbol is just a literal
                if (IndexOf(toReturn, symbol) == -1) {
                    toReturn.push_back(symbol);
                }
            }
        }
    }

    return toReturn;
}

string Replace(string haystack, string before, string after) {
    if (strcmp(haystack.c_str(), before.c_str()) == 0) {
        return after;
    }

    int len = before.length();

    for (int i = haystack.find(before); i != std::string::npos; i = haystack.find(before)) {
        haystack.replace(i, len, after);
    }

    return haystack;
}

float EvaluateClause(CNF* clause, unordered_map<string, int> model) {
    string s = " " + clause->toString() + " ";

    for (auto it = model.begin(); it != model.end(); it++) {
        s = Replace(s, it->first, to_string(it->second));
    }

    s = Replace(s, "-0", "1");
    s = Replace(s, "-1", "0");

    int truth = s.find(" 1 ");

    if (truth == -1) {
        s = Replace(s, " 0", "\0");
        if (strcmp(s.c_str(), " ") == 0) {
            return 0;
        } else {
            return 0.5;
        }
    } else {
        return 1;
    }
}

unordered_map<string, int> FindUnitClause(vector<CNF*> kb, unordered_map<string, int> model) {
    unordered_map<string, int> toReturn;

    for (CNF* clause : kb) {
        // Add padding
        string s = " " + clause->toString() + " ";

        // Repalce everything in the clause with its counterpart
        for (auto it = model.begin(); it != model.end(); it++) {
            s = Replace(s, it->first, to_string(it->second));
        }

        // Simplify negations
        s = Replace(s, "-0", "1");
        s = Replace(s, "-1", "0");

        // Is it true?
        int truth = s.find(" 1 ");

        if (truth == -1) {
            // Not already true

            s = Replace(s, " 0", "\0");
            stringstream ss(s);
            string data;
            while (ss >> data) {
                if (data[0] == '-') {
                    toReturn[data.substr(1)] = 0;
                } else {
                    toReturn[data] = 1;
                }
            }
        } else {
            continue;
        }
    }

    return toReturn;
}

void PrintModel(unordered_map<string, int> model) {
    for (auto it = model.begin(); it != model.end(); it++) {
        if (it->second == 1) {
            cout << it->first << endl;
        }
    }
}

int iterations = 0;

bool DPLL(vector<CNF*> kb, vector<string> symbols, unordered_map<string, int> model) {
    iterations++;
    int valid = 0, k = kb.size();

    // Check the validity of the current model
    for (CNF* clause : kb) {
        float eval = EvaluateClause(clause, model);
        if (eval == 0) {
            return false;
        } else if (eval == 1) {
            valid++;
        }
    }

    if (valid == k) {
        cout << "Completed in " << iterations << " iterations!" << endl;
        PrintModel(model);
        return true;
    }

    unordered_map<string, int> unit = FindUnitClause(kb, model);
    if (unit.size() == 1) {
        model[unit.begin()->first] = unit.begin()->second;
        return DPLL(kb, symbols, model);
    }

    string symbol = symbols.at(0);
    vector<string> next;

    for (int i = 1, s = symbols.size(); i < s; i++) {
        next.push_back(symbols.at(i));
    }

    unordered_map<string, int> T(model);
    T[symbol] = 1;

    unordered_map<string, int> F(model);
    F[symbol] = 0;

    return DPLL(kb, next, T) || DPLL(kb, next, F);

}

// Main code
int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Second parameter must be a file name" << endl;
        return 1;
    }

    // Get user input
    ifstream file;
    file.open(argv[1]);

    // Get the knowledge base
    vector<CNF*> kb;
    string line;

    while (getline(file, line)) {
        kb.push_back(new CNF(line));
    }

    // Get the propositional symbols
    vector<string> symbols = UniqueSymbols(kb);

    unordered_map<string, int> model;

    DPLL(kb, symbols, model);

    file.close();

    return 0;
}