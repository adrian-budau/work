#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

struct Pair {
    Pair(int _first = 0, int _second = 0, int _count = 1):
            first(_first), second(_second), count(_count) {}
    Pair(pair<int, int> p):
            Pair(p.first, p.second) {}

    bool operator==(const Pair& that) const {
        return first == that.first && second == that.second;
    }

    int first, second;
    int count;
};

struct Fraction {
    Fraction(int64_t _num = 0, int64_t _den = 1):
            num(_num), den(_den) {
        if (den < 0) {
            den = -den;
            num = -num;
        }
    }

    static Fraction when(Pair first, Pair second) {
        if (first.second == second.second)
            return Fraction(-1, 1);
        return Fraction(second.first - first.first, first.second - second.second);
    }

    bool operator<(const Fraction& that) const {
        return num * that.den < den * that.num;
    }

    bool operator<=(const Fraction& that) const {
        return num * that.den <= den * that.num;
    }

    int64_t num, den;
};

int main() {
    ifstream cin("campion.in");
    ofstream cout("campion.out");

    int N, T; cin >> N >> T;
    vector< pair<int, int> > A(N);
    for (int i = 0; i < N; ++i)
        cin >> A[i].first >> A[i].second;

    sort(A.begin(), A.end(), [&](pair<int, int> a, pair<int, int> b) {
        if (a.second != b.second)
            return a.second < b.second;
        return a.first < b.first;
    });

    vector<Pair> moments;
    for (int i = 0; i < N; ++i) {
        if (!moments.empty() && moments.back() == A[i]) {
            ++moments.back().count;
            continue;
        }

        while (!moments.empty() && Fraction::when(moments.back(), A[i]) < Fraction(0, 1))
            moments.pop_back();

        while (moments.size() > 1 && Fraction::when(moments.back(), A[i]) < Fraction::when(moments[moments.size() - 2], moments.back()))
            moments.pop_back();

        if (!moments.empty() && Fraction(T, 1) < Fraction::when(moments.back(), A[i]))
            continue;
        moments.push_back(A[i]);
    }

    int answer = 0;
    for (auto &p : moments)
        answer += p.count;
    cout << answer << "\n";
}
