#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    ifstream cin("timp.in");
    ofstream cout("timp.out");

    int N, K; cin >> N >> K;

    vector<int> answer;
    N -= K;
    while (N != 0 && K != 0)
        if (N < K) {
            answer.push_back(0);
            tie(N, K) = make_pair(K - N, 2 * N);
        } else {
            answer.push_back(1);
            tie(N, K) = make_pair(2 * K, N - K);
        }
    reverse(answer.begin(), answer.end());
    cout << answer.size() << "\n";
    for (auto &p : answer)
        cout << p << "\n";
}
