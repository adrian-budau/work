#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <limits>
#include <cassert>

using namespace std;

int main() {
    ifstream cin("pcb.in");
    ofstream cout("pcb.out");

    int N, M, X;
    assert(cin >> N >> M >> X);
    assert(1 <= N and N <= 100000);
    assert(1 <= M and M <= 200000);
    assert(1 <= X && X <= N);

    vector< vector<int> > edges(N + 1);
    for (int i = 0; i < M; ++i) {
        int A, B;
        assert(cin >> A >> B);
        assert(1 <= A && A <= B && B <= N);
        edges[A - 1].push_back(B);
        edges[B].push_back(A - 1);
    }

    queue<int> Q;
    Q.push(X);
    vector<int> distance(N + 1, numeric_limits<int>::max());
    distance[X] = 0;
    while (!Q.empty()) {
        int now = Q.front();
        Q.pop();
        if (now == 0) {
            cout << distance[0] << "\n";
            return 0;
        }

        for (auto &next : edges[now])
            if (distance[next] > distance[now] + 1) {
                distance[next] = distance[now] + 1;
                Q.push(next);
            }
    }
    cout << "-1\n";
}
