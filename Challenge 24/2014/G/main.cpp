#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <cstring>
#include <src/vectors.hpp>

using namespace std;
using namespace inputGenerator;

const int maxn = 55000;

vector<int> E1[maxn], E2[maxn];
int R1[maxn], R2[maxn], B1[maxn], B2[maxn];
int size1[maxn], size2[maxn], size1b[maxn], size2b[maxn];
int sumR1[maxn], sumR2[maxn];

int heights1[maxn], heights2[maxn];

int height = 0;
int max_height = 0;

void dfs(vector<int> E[], int size[], int R[], int sumR[], int node, int heights[]) {
    max_height = max(max_height, ++height);
    heights[node] = height;
    size[node] = 1;
    for (auto &x : E[node]) {
        dfs(E, size, R, sumR, x, heights);
        size[node] += size[x];
        sumR[node] += R[x];
    }
    --height;
}

bool canRemove(int node) {
    int i;
    for (i = node; B1[i] != i; i = B1[i])
        if (size1[i] == 0)
            return false;
    if (size1[i] == 0)
        return false;

    for (i = node; B2[i] != i; i = B2[i])
        if (size2[i] == 0)
            return false;
    if (size2[i] == 0)
        return false;
    return true;
}

void remove(int node) {
    int i;
    for (i = node; B1[i] != i; i = B1[i])
        --size1[i];
    --size1[i];
    for (i = node; B2[i] != i; i = B2[i])
        --size2[i];
    --size2[i];
}

int main() {
    ifstream cin("input");

    int N; cin >> N;
    assert(N < maxn);
    int parent1 = 0, parent2 = 0;
    for (int i = 0; i < N; ++i) {
        int bw, bu, rw, ru; cin >> bw >> bu >> rw >> ru;

        if (bw != i)
            E1[bw].push_back(i);
        else
            parent1 = i;
        if (bu != i)
            E2[bu].push_back(i);
        else
            parent2 = i;

        B1[i] = bw;
        B2[i] = bu;
        R1[i] = rw;
        R2[i] = ru;
    }

    dfs(E1, size1, R1, sumR1, parent1, heights1);
    dfs(E2, size2, R2, sumR2, parent2, heights2);

    for (int i = 0; i < N; ++i) {
        if (sumR1[i] == 0)
            assert(sumR1[i] <= R1[i]);
        if (sumR2[i] == 0)
            assert(sumR2[i] <= R2[i]);
        assert(size1[i] >= R1[i]);
        assert(size2[i] >= R2[i]);
        size1[i] -= R1[i];
        size2[i] -= R2[i];
    }

    vector<int> nodes(N);
    for (int i = 0; i < N; ++i)
        nodes[i] = i;
    vector<int> answer;
    sort(nodes.begin(), nodes.end(), [&](int x, int y) {
        if (heights2[x] != heights2[y])
            return heights2[x] < heights2[y];
        return heights1[x] < heights1[y];
    });

    for (int test = 0; test < 1000000; ++test) {
        vector<int> now;

        memcpy(size1b, size1, sizeof(int) * (N + 1));
        memcpy(size2b, size2, sizeof(int) * (N + 1));

        for (int i = 0; i < N; ++i)
            if (canRemove(nodes[i])) {
                now.push_back(nodes[i]);
                remove(nodes[i]);
            }

        if (now.size() > answer.size())
            answer = now;

        memcpy(size1, size1b, sizeof(int) * (N + 1));
        memcpy(size2, size2b, sizeof(int) * (N + 1));
        randomShuffle(nodes.begin(), nodes.end());
    }

    cout << answer.size() << "\n";
    for (auto &x : answer)
        cout << x << " ";
    cout << "\n";

    ofstream out("G4.out");
    out << answer.size() << "\n";
    for (auto &x : answer)
        out << x << " ";
    out << "\n";
}
