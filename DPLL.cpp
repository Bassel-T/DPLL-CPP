#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <unordered_map>
#include <stack>

#include "parser.h"

using namespace std;

/*
False = 0
Unknown = 0.5
True = 1
*/

struct HistEntry {
public:
string label;
int boolean;
bool isUnit;

HistEntry(string l, int b, bool i) {
    label = l;
    boolean = b;
    isUnit = i;
}
};

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
        s = Replace(s, " " + it->first, " " + to_string(it->second));
        s = Replace(s, "-" + it->first, "-" + to_string(it->second));
    }

    s = Replace(s, " -0", " 1");
    s = Replace(s, " -1", " 0");
 
    int truth = s.find(" 1 ");

    if (truth == std::string::npos) {
        s = Replace(s, " 0", "");
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
    for (CNF* clause : kb) {
        // Add padding
        string s = " " + clause->toString() + " ";
        unordered_map<string, int> toReturn;

        // Repalce everything in the clause with its counterpart
        for (auto it = model.begin(); it != model.end(); it++) {
            s = Replace(s, " " + it->first + " ", " " + to_string(it->second) + " ");
            s = Replace(s, "-" + it->first + " ", "-" + to_string(it->second) + " ");
        }

        // Simplify negations
        s = Replace(s, " -0", " 1");
        s = Replace(s, " -1", " 0");

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

        if (toReturn.size() == 1) {
            return toReturn;
        }
    }

    unordered_map<string, int> empty;
    return empty;
}

bool Contains(vector<CNF*> haystack, CNF* needle) {
    for (CNF* clause : haystack) {
        if (strcmp(clause->toString().c_str(), needle->toString().c_str()) == 0) {
            return true;
        }
    }

    return false;
}

bool ModelContains(unordered_map<string, int> haystack, string needle) {
    for (auto it : haystack) {
        if (strcmp(it.first.c_str(), needle.c_str()) == 0) {
            return true;
        }
    }

    return false;
}

void PrintModel(unordered_map<string, int> model, vector<string> symbols) {
    for (string i : symbols) {
        if (ModelContains(model, i)) {
            if (model[i] == 1) {
                cout << i << endl;
            } else {
                cout << "-" << i << endl;
            }
        } else {
            cout << "?" << i << "?" << ", ";
        }
    }

    cout << endl << endl;
}


int iter = 0;
int MAX_ITERS = 100000;
int nextChange = 0;
int recurseSkip = 0;

bool DPLL_Iterative(vector<CNF*> kb, vector<string> symbols, unordered_map<string, int> model) {
    int kbSize = kb.size();
    stack<HistEntry> history;

    while (iter++ < MAX_ITERS) {
        // Check the validity of the current model
        int valid = 0;
        bool falseFound = false;
        cout << "Iterations: " << iter << endl;


        for (CNF* clause : kb) {
            float eval = EvaluateClause(clause, model);
            if (eval == 0) {
                // There was something wrong
                // get top value from stack
                HistEntry latest = history.top();
                history.pop();

                // Find the next boolean that can be flipped
                while ((latest.boolean == 1 || latest.isUnit)) {
                    model.erase(latest.label);
                    latest = history.top();
                    history.pop();
                        if (latest.isUnit) { recurseSkip--; }
                        else {nextChange--;}
                }

                // Flip it
                latest.boolean = 1;
                history.push(latest);
                model[latest.label] = 1;
                falseFound = true;
                break;
            } else if (eval == 1) {
                valid++;
            }
        }

        if (falseFound) {
            continue;
        }

        if (valid == kbSize) {
            cout << "Completed in " << iter << " iterations!" << endl;
            PrintModel(model, symbols);
            return true;
        }

        // Unit Heuristic
        unordered_map<string, int> unit = FindUnitClause(kb, model);
        if (unit.size() == 1) {
            if (strcmp(unit.begin()->first.c_str(), " ") != 0) {
                HistEntry nextVal(unit.begin()->first, unit.begin()->second, true);
                history.push(nextVal);
                model[unit.begin()->first] = unit.begin()->second;
                continue;
                }
        }

        // TODO : Check if the next item is already in the model
        // While in model, increment nextChange
        while (ModelContains(model, symbols[nextChange + recurseSkip])) {
            recurseSkip++;
        }

        HistEntry newValue(symbols[nextChange + recurseSkip], 0, false);
        history.push(newValue);
        model[newValue.label] = 0;

        nextChange++;

    }

    return false;
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
        CNF* input = new CNF(line);
        if (!Contains(kb, input)) {
            kb.push_back(input);
        }
    }

    // Get the propositional symbols
    vector<string> symbols = UniqueSymbols(kb);

    unordered_map<string, int> model;

    if (!DPLL_Iterative(kb, symbols, model)) {
        cout << "Unsatisfiable!" << endl;
    }

    file.close();

    return 0;
}