#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main() {
    ifstream cin("speeding.in");
    ofstream cout("speeding.out");

    int N, M; cin >> N >> M;

    vector< pair<int, int> > A(N), B(M);
    for (int i = 0; i < N; ++i)
        cin >> A[i].first >> A[i].second;

    for (int i = 0; i < M; ++i)
        cin >> B[i].first >> B[i].second;

    int answer = 0;
    for (int i = 0, startA = 0; i < N; startA += A[i++].first)
        for (int j = 0, startB = 0; j < M; startB += B[j++].first) {
            if (startA + A[i].first <= startB)
                continue;
            if (startB + B[j].first <= startA)
                continue;
            answer = max(answer, B[j].second - A[i].second);
        }

    cout << answer << "\n";
}
