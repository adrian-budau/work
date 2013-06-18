#include <iostream>
#include <fstream>
#include <algorithm>
#include <cassert>

#define INPUT_GENERATOR_DEBUG
#include <inputGenerator.hpp>

#define read(X) cout << #X << " = "; cin >> X;

using namespace std;
using namespace inputGenerator;

void gen(int N, int M, int V) {
    auto G = undirectedGraph(N, M, Boolean::True);

    vector<int> A(N);
    int64_t sum = 0;
    for (int i = 0; i < N; ++i) {
        A[i] = randomInt(0, V);
        sum += A[i];
    }

    vector<int> B(N);
    for (int i = 0; i < N; ++i) {
        int64_t least = sum - static_cast<int64_t>(N - i - 1) * V;
        int64_t most = sum;

        least = max<int64_t>(0, least);
        most = min<int64_t>(V, most);
        B[i] = randomInt(least, most);
        sum -= B[i];
    }

    shuffle(B);

    ofstream in("input");
    in << N << " " << V << " " << M << "\n";
    for (int i = 0; i < N; ++i)
        in << A[i] << " ";
    in << "\n";

    for (int i = 0; i < N; ++i)
        in << B[i] << " ";
    in << "\n";

    for (auto &edge : shuffle(G.edges())) {
        int x = edge.from().index();
        int y = edge.to().index();

        swap(x, y);
        in << x + 1 << " " << y + 1 << "\n";
    }
}

int main() {
    int TESTS, MAXN, MAXM, MAXV;

    read(TESTS); read(MAXN); read(MAXM); read(MAXV);


    for (int test = 1; test <= TESTS; ++test) {
        int N, M, V;
        if (test == TESTS) {
            N = MAXN;
            M = MAXM;
            V = MAXV;
        } else {
            N = randomInt(2, MAXN);
            M = randomInt(N - 1, min(MAXM, N * (N - 1) / 2));
            V = randomInt(1, MAXV);
        }
        gen(N, M, V);

        assert(system("./main < input > output") == 0);

        cout << "Case #" << test << ": ";
        cout.flush();
        if (system("./verif") != 0) {
            break;
        }

        cout << "OK!\n";
    }
}
