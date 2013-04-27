#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int main() {
    vector<int> A(26, 0);

    string S; cin >> S;
    for (int i = 0; i < 26; ++i)
        A[i] = count(S.begin(), S.end(), 'a' + i);

    double answer = 0;
    double N = S.size();
    for (int i = 0; i < 26; ++i) {
        double expected = A[i];
        if (expected < 1e-9)
            continue;
        for (int j = 0; j < int(S.size()); ++j) {
            double tmp = (expected / (N - j));
            answer += tmp * tmp;

            expected = expected / (N - j) * (expected - 1) + expected * (N - j - expected) / (N - j);
        }
    }

    cout.setf(ios::fixed, ios::floatfield);
    cout.precision(12);
    cout << answer << "\n";
}
