#include <iostream>
#include <algorithm>

using namespace std;

istream& operator>>(istream& in, pair<double, double> &A) {
    in >> A.first >> A.second;
    return in;
}

typedef pair<double, double> point;
point A[3], B[3];

int dist(point A, point B) {
    return (A.first - B.first) * (A.first - B.first) + (A.second - B.second) * (A.second - B.second);
}

int main() {
    for (int i = 0; i < 3; ++i)
        cin >> A[i];

    for (int i = 0; i < 3; ++i)
        cin >> B[i];

    vector<int> X, Y;
    for (int i = 0; i < 3; ++i)
        for (int j = i + 1; j < 3; ++j) {
            X.push_back(dist(A[i], A[j]));
            Y.push_back(dist(B[i], B[j]));
        }

    sort(X.begin(), X.end());
    sort(Y.begin(), Y.end());

    for (int i = 1; i < 3; ++i)
        if (1LL * X[i] * Y[0] != 1LL * Y[i] * X[0]) {
            cout << "NO\n";
            return 0;
        }

    cout << "YES\n";
}
