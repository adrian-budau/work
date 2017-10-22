#include <iostream>
#include <algorithm>
#include <fstream>
#include <cassert>
#include <set>
#include <vector>

using namespace std;

void result(string message, int score = 0) {
    cerr << message;
    cout << score;
    exit(0);
}

int main() {
    ifstream in("danger.in");
    ifstream out("danger.out");
    ifstream ok("danger.ok");

    if (!out)
        result("Fisier de iesire lipsa");

    int N, M;
    assert(in >> N >> M);
    vector< multiset<int> > S(N);
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j) {
            int x;
            assert(in >> x);
            S[i].emplace(x);
        }

    int correct = numeric_limits<int>::min();
    for (int i = 0; i < M; ++i) {
        vector<int> V(N);
        for (int i = 0; i < N; ++i)
            assert(ok >> V[i]);
        sort(V.begin(), V.end());
        correct = max(correct, V[N - 1] + V[N - 2]);
    }
    int output = numeric_limits<int>::min();
    for (int i = 0; i < M; ++i) {
        vector<int> V(N);
        for (int j = 0; j < N; ++j) {
            if (!(out >> V[j]))
                result("Fisier de iesire incomplet");
            if (!S[j].count(V[j]))
                result("Wrong answer");
            S[j].erase(S[j].find(V[j]));
        }
        sort(V.begin(), V.end());
        output = max(output, V[N - 1] + V[N - 2]);
    }

    assert(output >= correct);
    if (output != correct)
        result("Wrong answer");
    result("OK", 5);
}
