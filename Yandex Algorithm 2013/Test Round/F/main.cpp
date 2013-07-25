#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    ifstream cin("taxi.in");
    ofstream cout("taxi.out");

    int64_t M, D; int N;
    cin >> M >> D >> N;

    vector<int64_t> X(N);
    for (int i = 0; i < N; ++i)
        cin >> X[i];

    vector<int64_t> Y;
    int64_t need = -1;

    for (auto &v : X)
        if (v >= M - D and (v < need or need == -1))
            need = v;

    int64_t enough = need;

    for (auto &v : X)
        if (v == need) {
            need = -1;
            continue;
        } else
            Y.push_back(v);
    swap(X, Y);
    sort(X.begin(), X.end());

    int64_t position = 0;

    int answer = 0;
    while (position < M) {
        if (X.size() == 0 and enough == -1) {
            cout << "0\n";
            return 0;
        }

        int64_t v = 0;
        need = M - position;
        if (position < D)
            need += (D - position);
        if (X.size() and enough < need) {
            v = X.back();
            X.pop_back();
        } else {
            v = enough;
            enough = -1;
        }

        if (position < D)
            v -= D - position;
        else
            v -= position - D;
        if (v <= 0)
            continue;
        position += v;
        answer++;
    }

    cout << answer << "\n";
}
