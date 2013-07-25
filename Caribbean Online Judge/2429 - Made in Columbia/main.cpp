#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

int main() {
    while (true) {
        int N; cin >> N;

        if (N == 0)
            break;

        multiset<int> L, R;
        long double sum = 0;
        for (int i = 0; i < N; ++i) {
            int x; cin >> x;
            if (i == 0)
                L.insert(x);
            else {
                if (x >= *L.rbegin())
                    R.insert(x);
                else
                    L.insert(x);

                while (L.size() > R.size() + 1) {
                    R.insert(*L.rbegin());
                    L.erase(L.find(*L.rbegin()));
                }

                while (R.size() > L.size()) {
                    L.insert(*R.begin());
                    R.erase(R.begin());
                }
            }

            sum += *L.rbegin();
        }

        sum /= N;

        cout.setf(ios::fixed, ios::floatfield);
        cout.precision(2);

        cout << sum << "\n";
    }
}
