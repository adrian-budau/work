#include <iostream>
#include <algorithm>
#include <vector>
#include <set>

using namespace std;

int playWar(vector<double> A, vector<double> B) {
    set<double> left(A.begin(), A.end());
    set<double> right(B.begin(), B.end());

    while (left.size() && *left.begin() < *right.rbegin()) {
        auto which = right.lower_bound(*left.begin());
        right.erase(which);
        left.erase(left.begin());
    }

    return left.size();
}

int playDeceivedWar(vector<double> A, vector<double> B) {
    set<double> left(A.begin(), A.end());
    set<double> right(B.begin(), B.end());

    int score = 0;
    while (left.size()) {
        if (*left.begin() > *right.begin()) {
            left.erase(left.begin());
            right.erase(right.begin());
            ++score;
            continue;
        }

        auto end = right.end();
        --end;
        right.erase(end);
        left.erase(left.begin());
    }
    return score;
}

int main() {
    int T; cin >> T;
    for (int test = 1; test <= T; ++test) {
        int N; cin >> N;
        vector<double> A(N), B(N);
        for (int i = 0; i < N; ++i)
            cin >> A[i];
        for (int i = 0; i < N; ++i)
            cin >> B[i];

        cout << "Case #" << test << ": ";
        cout << playDeceivedWar(A, B) << " " << playWar(A, B) << "\n";
    }
}
