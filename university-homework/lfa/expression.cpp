#include <iostream>
#include <string>

#include "automaton.h"

using namespace std;

string dictionary = "abcdefghijklmnopqrstuvwxyz";

Automaton solve(string &S, int &position, int level) {
    if (level == 0) {
        Automaton aux = solve(S, position, 1);
        while (position < int(S.size()) and S[position] == '+') {
            Automaton next = solve(S, ++position, 1);
            aux += next;
        }
        return aux;
    }

    if (level == 1) {
        Automaton aux = solve(S, position, 2);
        while (position < int(S.size()) and S[position] == '|') {
            Automaton next = solve(S, ++position, 2);
            aux |= next;
        }

        return aux;
    }

    if (level == 2) {
        Automaton aux = solve(S, position, 3);
        while (position < int(S.size()) and S[position] == '*') {
            aux.star();
            ++position;
        }

        return aux;
    }

    if (S[position] == '(') {
        Automaton aux = solve(S, ++position, 0);
        ++position;
        return aux;
    }

    Automaton aux(2, dictionary);
    aux.setFinal(1);
    aux.setStart(0);
    aux.addTransition(0, 1, S[position]);
    ++position;
    return aux;
}

int main() {
    string S; getline(cin, S);

    int poz = 0;
    Automaton T = solve(S, poz, 0);
    Automaton V = T.removeEmptyTransitions().makeDeterminist().minimize();

    cout << T << "\n";
    cout << T.removeEmptyTransitions().makeDeterminist() << "\n";
    cout << V << "\n";

    while (cin >> S)
        if (V.check(S))
            cout << "Bun\n";
        else
            cout << "Rau\n";
}
