#include <iostream>
#include <vector>

using namespace std;

int minTree(vector<int> A, int sigma) {
    vector<int> many(A.back() + 1, 0);
    for (auto &x : A) {
        ++many[x];
    }

    int answer = 0;
    for (int i = many.size() - 1; i > 0; --i) {
        answer += many[i];
        many[i - 1] = max(many[i - 1], (many[i] - 1) / sigma + 1);
    }
    return answer;
}

int maxTree(vector<int> A, int sigma) {
    vector<int> many(A.back() + 1, 0);
    for (auto &x : A) {
        ++many[x];
    }

    int answer = 0;
    for (int i = many.size() - 1; i > 0; --i) {
        int maximum = 1;
        for (int j = 0; j < i and maximum <= many[i]; ++j)
            maximum *= sigma;
        many[i] = min(many[i], maximum);
        answer += many[i];
        many[i - 1] += many[i];
    }

    return answer;
}

int main() {
    ios::sync_with_stdio(false);
    int T; cin >> T;

    for (int test = 1; test <= T; ++test) {
        int N, sigma; cin >> N >> sigma;

        vector<int> A(N);
        for (int i = 0; i < N; ++i)
            cin >> A[i];

        sort(A.begin(), A.end());
        cout << "Case #" << test << ":\n";
        cout << minTree(A, sigma) << " " << maxTree(A, sigma) << "\n";
    }
}
