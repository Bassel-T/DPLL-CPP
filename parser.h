/*
Figure out the structure of logic:
implies, not, or, and
(or a b (not c)) --> a b -c
*/

#include <vector>
#include <string>
#include <sstream>

using namespace std;

class CNF {
public:
    vector<string> expr;

    CNF(string s) {
        stringstream input(s);
        string data;
        while (input >> data) {
            expr.push_back(data);
        }
    }

    string toString() {
        string toReturn = expr.at(0);
        for (int i = 1, s = expr.size(); i < s; i++) {
            toReturn += " " ;
            toReturn += expr.at(i);
        }

        return toReturn;
    }

};