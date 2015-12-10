#include <iostream>
#include <vector>
#include <fstream>
#include <array>
#include <cassert>

using namespace std;

const int kParts = 180;

void rebuild(const vector<int> &V, array< vector<int>, kParts > &arr) {
    for (int i = 0; i < kParts; ++i)
        arr[i].clear();
    for (int i = 0; i < int(V.size()); ++i)
        arr[i / kParts].push_back(V[i]);
}

vector<int> copyFrom(const array< vector<int>, kParts> &arr) {
    int N = 0;
    for (auto &x : arr)
        N += x.size();

    vector<int> answer;
    answer.reserve(N);
    for (auto &x : arr)
        for (auto &y : x)
            answer.push_back(y);
    return answer;
}

int remove(array< vector<int>, kParts> &arr, int who) {
    for (auto &x : arr) {
        if (who < int(x.size())) {
            int v = x[who];
            x.erase(x.begin() + who);
            return v;
        }
        who -= x.size();
    }

    assert(false);
}

void insert(array< vector<int>, kParts> &arr, int where, int v) {
    for (auto &x : arr) {
        if (where <= int(x.size())) {
            x.insert(x.begin() + where, v);
            return;
        }
        where -= x.size();
    }
    assert(false);
}

int main() {
    ifstream cin("grid.in");
    ofstream cout("grid.out");

    int N, K; cin >> N >> K;
    assert(1 <= N && N <= 10 * 1000);
    assert(0 <= K && K <= 150 * 1000);

    array<array< vector<int>, kParts >, 3> elems;

    for (int i = 0; i < 3; ++i) {
        vector<int> V(N);
        for (int j = 0; j < N; ++j)
            V[j] = j + 1 + i * N;
        rebuild(V, elems[i]);
    }

    for (int i = 1; i <= K; ++i) {
        if (i % K == 0) {
            for (int j = 0; j < 3; ++j) {
                auto V = copyFrom(elems[j]);
                rebuild(V, elems[j]);
            }
        }

        int x1, y1, x2, y2; cin >> x1 >> y1 >> x2 >> y2;
        assert(0 <= x1 && x1 <= 2);
        assert(0 <= x2 && x2 <= 2);
        assert(0 <= y1 && 0 <= y2);

        auto v = remove(elems[x1], y1);

        insert(elems[x2], y2, v);
    }

    for (int i = 0; i < 3; ++i) {
        auto V = copyFrom(elems[i]);
        for (auto &x : V)
            cout << x << " ";
        cout << "\n";
    }
}
