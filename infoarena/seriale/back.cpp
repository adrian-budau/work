#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

vector<int> watch(const vector<int> &A, const vector<int> &B) {
    set<int> V;
    for (auto &x : A)
        V.insert(x);

    for (int index = 0; index < int(B.size()); ++index) {
        if (index % 2 == 0) {
            V.erase(*V.rbegin());
        } else {
            V.erase(V.begin());
        }

        V.emplace(B[index]);
    }

    vector<int> left; left.reserve(A.size());
    for (auto &x : A)
        if (V.count(x))
            left.push_back(x);
    return left;
}

int main() {
    ifstream cin("seriale.in");
    ofstream cout("seriale.out");

    int N, K; cin >> N >> K;

    vector<int> A(N);
    for (int i = 0; i < N; ++i)
        cin >> A[i];
    vector<int> B(K);
    for (int i = 0; i < K; ++i)
        cin >> B[i];
    sort(B.begin(), B.end());
    if (is_sorted(A.begin(), A.end())) {
        cout << 0 << "\n";
        return 0;
    }

    vector<int> C;
    C.reserve(K);
    int answer = K + 1;
    for (int i = 1; i < (1 << K); ++i) {
        C.clear();
        for (int j = 0; j < K; ++j)
            if ((1 << j) & i)
                C.push_back(B[j]);

        do {
            auto left = watch(A, C);
            if (is_sorted(left.begin(), left.end()))
                answer = min(answer, int(C.size()));

        } while (next_permutation(C.begin(), C.end()));
    }

    if (answer > K)
        answer = -1;
    cout << answer << "\n";
}

