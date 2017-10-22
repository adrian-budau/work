#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

int smallest(int X, int Y, bool first = false) {
    if (!first)
        return min(X, Y);
    if (X == 0)
        return Y;
    return X;
}

bool operator<(const vector<int>& A, const vector<int>& B) {
    if (A.size() != B.size())
        return A.size() < B.size();
    for (int i = 0; i < int(A.size()); ++i)
        if (A[i] != B[i])
            return A[i] < B[i];
    return false;
}

void print(const vector<int> &digits) {
    string answer;
    for (auto &x : digits)
        answer += char(x + '0');
    cout << answer << "\n";
}

int main() {
    ios::sync_with_stdio(false);

    string N; int X, Y; cin >> N >> X >> Y;
    if (Y < X)
        swap(X, Y);

    vector<int> digits;
    for (auto &c : N)
        digits.emplace_back(c - '0');

    if (digits.size() == 1 && smallest(X, Y, true) > digits[0]) {
        cout << "-1\n";
        return 0;
    }

    // check if we can do with same length
    int D = digits.size();
    vector<int> maybe;
    for (int i = 0; i < D; ++i)
        maybe.push_back(smallest(X, Y, i == 0));
    if (digits < maybe) {
        maybe.clear();
        for (int i = 0; i < D - 1; ++i)
            maybe.push_back(max(X, Y));
        print(maybe);
        return 0;
    }

    vector<bool> is_ok(D, false);
    for (int i = D - 1; i >= 0; --i) {
        if (maybe[i] < digits[i])
            is_ok[i] = true;
        else if (maybe[i] == digits[i]) {
            if (i == D - 1 || is_ok[i + 1])
                is_ok[i] = true;
            else
                is_ok[i] = false;
        } else {
            is_ok[i] = false;
        }
    }

    maybe.clear();
    bool still_equal = true;
    for (int i = 0; i < D; ++i) {
        if (Y < digits[i])
            still_equal = false;

        if (!still_equal) {
            maybe.push_back(Y);
            continue;
        }

        if (Y == digits[i]) {
            if (i == D - 1 || is_ok[i + 1]) {
                maybe.push_back(Y);
                continue;
            }
        }

        assert(X <= digits[i]);
        if (X < digits[i])
            still_equal = false;
        maybe.push_back(X);
    }
    print(maybe);
}
