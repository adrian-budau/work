#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <algorithm>

using namespace std;

void result(string message, int points) {
    cerr << message;
    cout << points;
    exit(0);
}

int main() {
    ifstream in("permsplit.in");

    if (not in.is_open())
        result("Fisier de intrare lipsa", 0);

    ifstream out("permsplit.out");

    if (not out.is_open())
        result("Fisier de iesire lipsa", 0);

    int N;
    if (!(in >> N))
        result("Fisier de intrare gresit", 0);

    vector<int> V(N);
    vector<int> begin(N), end(N);

    for (int i = 0; i < N; ++i) {
        if (!(in >> begin[i]))
            result("Fisier de intrare incomplet", 0);
        V[i] = i;
        end[i] = begin[i];
    }

    vector<int> Where(N - 1);
    for (int i = 0; i < N - 1; ++i) {
        if (!(out >> Where[i]))
            result("Fisier de iesire incomplet", 0);
        if (Where[i] < 1 || Where[i] >= N)
            result("Wrong answer", 0);
    }

    reverse(Where.begin(), Where.end());
    for (int i = 0; i < N - 1; ++i) {
        int where = Where[i];
        --where;
        if (V[where] == V[where + 1])
            result("Wrong answer", 0);


        int left1 = begin[V[where]], right1 = end[V[where]];
        int left2 = begin[V[where + 1]], right2 = end[V[where + 1]];
        if (right1 != left2 - 1 and right2 != left1 - 1)
            result("Wrong answer", 0);

        int who;
        if (right1 - left1 < right2 - left2) {
            who = V[where + 1];
            for (int j = where - (right1 - left1); j <= where; ++j)
                V[j] = who;
        } else {
            who = V[where];
            for (int j = where + 1; j <= (where + 1 + (right2 - left2)); ++j)
                V[j] = who;
        }

        begin[who] = min(left1, left2);
        end[who] = max(right1, right2);
    }

    result("OK", 10);
}

