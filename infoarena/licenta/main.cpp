#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    ifstream cin("licenta.in");
    ofstream cout("licenta.out");

    int K; cin >> K;
    int N; cin >> N;
    vector< pair<int, int> > A(N);
    for (int i = 0; i < N; ++i)
        cin >> A[i].first >> A[i].second;

    int M; cin >> M;
    vector< pair<int, int> > B(M);
    for (int i = 0; i < M; ++i)
        cin >> B[i].first >> B[i].second;

    sort(A.begin(), A.end());
    sort(B.begin(), B.end());

    int x = 0, y = 0;
    while (x < N && y < M) {
        int overlap = min(A[x].second, B[y].second) - max(A[x].first, B[y].first);
        if (overlap >= K) {
            cout << max(A[x].first, B[y].first) << " " << max(A[x].first, B[y].first) + K << "\n";
            return 0;
        }

        if (A[x].second <= B[y].second)
            ++x;
        else
            ++y;
    }
    cout << "-1\n";
}
