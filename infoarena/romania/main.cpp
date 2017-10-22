#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <queue>

using namespace std;

int main() {
    ifstream cin("romania.in");
    ofstream cout("romania.out");

    int N, K; assert(cin >> N >> K);
    assert(4 <= N && N <= 100000);
    assert(1 <= K && K <= N - 3);
    vector<int> degree(N, 0);
    for (int i = 0; i < K; ++i) {
        int x; assert(cin >> x);
        assert(1 <= x && x <= N);
        degree[x - 1]++;
    }

    vector<int> left(N), right(N);
    queue<int> Q;
    for (int i = 0; i < N; ++i) {
        left[i] = (i + N - 1) % N;
        right[i] = (i + 1) % N;
        if (degree[i] > 0 && degree[right[i]] == 0)
            Q.push(i);
    }

    while (!Q.empty()) {
        int node = Q.front();
        Q.pop();

        int next = right[right[node]];
        cout << node + 1 << " " << next + 1 << "\n";

        // first remove his right
        right[node] = next;
        left[next] = node;
        --degree[node];
        if (degree[node] == 0 && degree[left[node]] > 0)
            Q.push(left[node]);
        else if (degree[node] > 0 && degree[right[node]] == 0)
            Q.push(node);
    }
}
