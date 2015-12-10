#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

int main() {
    ifstream cin("pizza.in");
    ofstream cout("pizza.out");
    int N; cin >> N;
    assert(1 <= N and N <= 5000);

    vector<int> V(N);
    for (int i = 0; i < N; ++i) {
        cin >> V[i];
        assert(1 <= V[i] and V[i] < 1000000);
    }

    V.insert(V.end(), V.begin(), V.end() - 1);

    vector<int> best(V.begin(), V.end()), nextBest(2 * N);
    for (int i = 1; i < N; ++i) {
        for (int j = 0; j < 2 * N - i - 1; ++j)
            nextBest[j] = max(V[j] - best[j + 1], V[j + i] - best[j]);
        swap(best, nextBest);
    }

    cout << *max_element(best.begin(), best.begin() + N) << "\n";
}
