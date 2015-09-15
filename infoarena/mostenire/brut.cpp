#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <set>

using namespace std;

int main() {
    ifstream cin("mostenire.in");
    ofstream cout("mostenire.out");

    int N; int64_t S; cin >> N >> S;

    vector<int> A(N);
    for (int i = 0; i < N; ++i)
        cin >> A[i];

    int T = 0;
    for_each(A.begin(), A.end(), [&](int x) {
        T += x;
    });

    bool found = false;
    for (int D = 0; D <= S; ++D) {
        for (int V = 0; V <= S; ++V) {
            if (T * V + T * D < S) {
                //cerr << "!";
                continue;
            }
            if (V * T > S) {
                //cerr << "+";
                continue;
            }
            if (V * A.back() > V * A.front() + D * A.front() + D) {
                //cerr << "+";
                continue;
            }
            int need = 0;
            for (int i = 0; i < N; ++i)
                need += max(V * A.back() - V * A[i] - D, 0);
            if (V * T + need > S) {
                //cerr << "+";
                continue;
            }

            multiset<pair<int, int> > have;
            vector<int> fin;
            for (int i = 0; i < N; ++i)
                have.emplace(V * A[i], D * A[i]);

            for (int i = 0; i < S - V * T; ++i) {
                int x, y;
                tie(x, y) = *have.begin();
                have.erase(have.begin());
                ++x; --y;
                if (y)
                    have.emplace(x, y);
                else
                    fin.emplace_back(x);
            }

            for (auto &p : have)
                fin.emplace_back(p.first);

            if (*min_element(fin.begin(), fin.end()) + D < *max_element(fin.begin(), fin.end()))
                ;//cerr << "-";
            else {
                //cerr << "0";
                if (!found) {
                    cout << D << "\n";
                    sort(fin.begin(), fin.end());
                    for (auto &x : fin)
                        cout << x << " ";
                    cout << "\n";
                    found = true;
                    return 0;
                }
            }
        }
        //cerr << "\n";
    }
}
