#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

bool better(int a, int b) {
    double first = b * log(a);
    double second = a * log(b);
    if (first - 1e-9 > second)
        return true;
    return false;
}

int main() {
    int T; cin >> T;

    while (T--) {
        int N; cin >> N;

        vector<int> A, B;
        for (int i = 0; i < N; ++i) {
            int x; cin >> x;
            if (x != 1)
                A.push_back(x);
        }

        int ones = 0, twos = 0;
        for (int i = 0; i < N; ++i) {
            int x; cin >> x;
            if (x == 1)
                ++ones;
            else if (x == 2)
                ++twos;
            else
                B.push_back(x);
        }

        sort(A.begin(), A.end());
        sort(B.begin(), B.end());

        double answer = 0;
        for (auto &value : A) {
            int j = lower_bound(B.begin(), B.end(), value, [&](int first, int second) {
                return !better(second, first);
            }) - B.begin();
            answer += double(B.size() - j + ones) / N;
            if (value == 3)
                answer += double(twos) / N;
        }

        cout.setf(ios::fixed, ios::floatfield);
        cout.precision(9);
        cout << answer << "\n";
    }
}
