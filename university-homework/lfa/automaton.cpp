#include <iostream>
#include <fstream>
#include <vector>

#include "automaton.h"

using namespace std;

int main() {
    ifstream cin("input");
    ofstream cout("output");

    Automaton T; cin >> T;

    cout << T << "\n";
    cout << T.removeEmptyTransitions() << "\n";

    cout << T.removeEmptyTransitions().makeDeterminist() << "\n";

    cout << T.removeEmptyTransitions().makeDeterminist().inverse().makeDeterminist().inverse() << "\n";
    cout << T.removeEmptyTransitions().makeDeterminist().minimize() << "\n";
}
