#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    ifstream cin("bucket.in");
    ofstream cout("bucket.out");

    int N; cin >> N;
    vector< pair<int, int> > V(N);
    int maxB = 0;
    for (int i = 0; i < N; ++i) {
        cin >> V[i].first >> V[i].second;
        maxB = max(maxB, V[i].second);
    }

    vector<int> answer;
    for (int B = 1; B <= N; ++B) {
        auto V2 = V;
        sort(V2.begin(), V2.end(), [&](pair<int, int> A, pair<int, int> B) {
            return A.second < B.second;
        });

        stable_sort(V2.begin(), V2.end(), [&](pair<int, int> A, pair<int, int> C) {
            if (A.first / B < C.first / B)
                return A.first / B < C.first / B;
            return A.second / B < C.second / B;
        });
        if (V == V2)
            answer.push_back(B);
    }

    cout << answer.size() << "\n";
    for (auto &x : answer)
        cout << x << " ";
    cout << "\n";
}
