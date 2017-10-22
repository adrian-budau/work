#include <iostream>
#include <algorithm>
#include <vector>
#include <limits>
#include <random>

using namespace std;

int main() {
    int N; cin >> N;

    vector<int> V(N * N);
    for (int i = 0; i < N * N; ++i)
        V[i] = i + 1;

    int value = N * N;
    vector< vector<int> > A(N, vector<int>(N));
    A[0][0] = value--;
    for (int i = 1; i < N; ++i) {
        for (int j = 0; j < i; ++j)
            A[i][j] = value--;
        for (int j = 0; j < i; ++j)
            A[j][i] = value--;
        A[i][i] = value--;
    }

    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            V[i * N + j] = A[i][j];
    int best = numeric_limits<int>::max();

    vector< vector<int> > answers;
    std::mt19937 r{std::random_device{}()};
    int steps = 0;
    do {
        if (steps > 1000000)
            break;
        ++steps;
        vector< vector<int> > A(N, vector<int>(N));
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                A[i][j] = V[i * N + j];

        int area = N * N + N * N * (N * N + 1) * 2;
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j) {
                if (i > 0)
                    area -= 2 * min(A[i][j], A[i - 1][j]);
                if (j > 0)
                    area -= 2 * min(A[i][j], A[i][j - 1]);
            }
        if (area < best) {
            answers.clear();
            best = area;
            answers.push_back(V);
        } else if (area == best)
            answers.push_back(V);
        shuffle(V.begin(), V.end(), r);
    } while (true);

    cout << best << "\n";
    cout << "With " << answers.size() << "\n";

    answers.resize(min<unsigned>(5, answers.size()));

    for (auto &v : answers) {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j)
                cout << v[i * N + j] << " ";
            cout << "\n";
        }
        cout << "\n";
    }
}
