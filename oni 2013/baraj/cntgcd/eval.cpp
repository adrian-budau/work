#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cassert>
#include <thread>

#include <inputGenerator.hpp>

#define READ(X) int X; cout << #X << " = "; cin >> X;

using namespace std;
using namespace inputGenerator;

void gen(const int &N, const int &D) {
    ofstream in("cntgcd.in");
    in << N << " " << D << "\n";
}

void eval(const string &S, long long &x) {
    assert(system(S.c_str()) == 0);
    ifstream out("cntgcd.out");
    out >> x;
}

int main() {
    READ(TESTS); READ(MAXN); READ(MAXD);
    for (int i = 1; i <= TESTS; ++i) {
        if (i == TESTS)
            gen(MAXN, MAXD);
        else {
            int N = randomInt(1, MAXN);
            int D = randomInt(1, min(MAXD, N));
            gen(N, D);
        }
      
        vector< pair<string, string> > people = {
            {"Mihai 100", "./cntgcd-mihai"},
            {"Adrian 100", "./cntgcd-adi"}, 
//            {"Mihai ciur", "./cntgcd-ciur"},
            {"Adrian Panaete - No idea", "./cntgcd-pa"}
//            {"Dragos brut", "./cntgcd-brut"}
        };

        cout << "Case #" << i << ": ";

        vector<long long> result(people.size(), -1);
        vector<thread> runners(people.size());

        for (size_t i = 0; i < result.size(); ++i) {
            runners[i] = thread(eval, people[i].second, ref(result[i]));
        }

        for (size_t i = 0; i < result.size(); ++i)
            runners[i].join();

        if (result != vector<long long>(result.size(), result.front())) {
            for (size_t i = 0; i < result.size(); ++i)
                cout << result[i] << "(" << people[i].first << ")    ";
            cout << "\n";
            break;
        }

        cout << result.front() << "\n";
    }
}
