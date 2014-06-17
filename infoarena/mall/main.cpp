#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <limits>

using namespace std;

int main() {
    ifstream cin("mall.in");
    ofstream cout("mall.out");

    int N, M; cin >> N >> M;

    vector<int> L(N), E(N), H(N), C(N);
    for (int i = 0; i < N; ++i)
        cin >> L[i] >> E[i] >> H[i] >> C[i];

    vector<int> now(M + 1, numeric_limits<int>::min() / 2);
    now[0] = 0;

    for (int i = 0; i < N; ++i) {
        vector<int> next(M + 1, numeric_limits<int>::min() / 2);

        deque<int> D;
        int too_many = numeric_limits<int>::min() / 2;

        for (int j = 0; j <= M; ++j) {
            while (D.size() and now[D.back()] <= now[j])
                D.pop_back();

            D.push_back(j);
            while (D.size() and D.front() <= j - C[i])
                D.pop_front();


            if (D.size())
                next[j] = max(next[j], now[D.front()] + L[i]);

            if (j >= C[i])
                next[j] = max(next[j], now[j - C[i]] + E[i]);

            if (j > C[i])
                too_many = max(too_many, now[j - C[i] - 1]);

            next[j] = max(next[j], too_many + H[i]);
        }

        swap(next, now);
    }

    cout << now[M] << "\n";
}
