#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <algorithm>
#include <cassert>
#include <set>

using namespace std;

struct Vector {
    Vector(int _X = 0, int _Y = 0):
            X(_X), Y(_Y) {
    }

    Vector operator+(const Vector& that) const {
        return {X + that.X, Y + that.Y};
    }

    Vector& operator+=(const Vector& that) {
        X += that.X;
        Y += that.Y;
        return *this;
    }

    Vector operator-(const Vector& that) const {
        return {X - that.X, Y - that.Y};
    }

    bool operator<(const Vector& that) const {
        if (X * that.Y == Y * that.X)
            return X < that.X;

        return X * that.Y > Y * that.X;
    }

    bool operator==(const Vector& that) const {
        return X == that.X && Y == that.Y;
    }

    int X, Y;
};

struct Fraction {
    Fraction(int64_t _num = 0, int _den = 1):
            num(_num),
            den(_den) {
    }

    int64_t num;
    int den;
};

bool operator<=(const Fraction &first, const Fraction& second) {
    return first.num * second.den <= first.den * second.num;
}

bool operator<=(const Fraction &first, int second) {
    return first <= Fraction(second);
}

bool operator<=(int first, const Fraction &second) {
    return Fraction(first) <= second;
}

class VectorSum {
  public:
    VectorSum(int size):
        m_size(size),
        m_fenwick_tree(size + 1),
        m_data(size) {
        }

    void update(int position, Vector value) {
        auto diff = value - m_data[position];
        m_data[position] = value;

        for (position++; position <= m_size; position += (position & -position))
            m_fenwick_tree[position] += diff;
    }

    Fraction getY(int X) {
        if (X == 0)
            return Fraction(0, 1);

        Vector vectorLowerBound, vectorUpperBound;
        tie(vectorLowerBound, vectorUpperBound) = lowerUpperBound(X);

        auto diff = vectorUpperBound - vectorLowerBound;
        if (diff == Vector(0, 0)) // no solution, let's just return -1 as a fraction
            return Fraction(-1, 1);

        return Fraction(int64_t(diff.Y) * (X - vectorLowerBound.X) + int64_t(diff.X) * vectorLowerBound.Y, diff.X);
    }

  private:
    pair<Vector, Vector> lowerUpperBound(int X) {
        int step;
        for (step = 1; step < m_size; step <<= 1);
        Vector now;
        int index;
        for (index = 0; step > 0; step >>= 1)
            if (index + step <= m_size && now.X + m_fenwick_tree[index + step].X < X) {
                index += step;
                now += m_fenwick_tree[index];
            }

        if (index + 1 <= m_size)
            return make_pair(now, now + m_data[index]);

        return make_pair(now, now);
    }

    int m_size;
    vector<Vector> m_fenwick_tree;
    vector<Vector> m_data;
};

int main() {
    ifstream cin("combl.in");
    ofstream cout("combl.out");

    int N; cin >> N;
    assert(1 <= N && N <= 150000);
    vector< pair<int, pair<int, int> > > ops(N);
    vector<Vector> updates;
    int ops1 = 0;
    for (int i = 0; i < N; ++i) {
        cin >> ops[i].first >> ops[i].second.first >> ops[i].second.second;
        assert(1 <= ops[i].first && ops[i].first <= 3);
        if (ops[i].first < 3)
            assert(1 <= ops[i].second.first && 1 <= ops[i].second.second &&
                   ops[i].second.first <= 10000 && ops[i].second.second <= 10000);
        else
            assert(0 <= ops[i].second.first && 0 <= ops[i].second.second &&
                   ops[i].second.first <= 1500000000 && ops[i].second.second <= 1500000000);
        if (ops[i].first == 1) {
            updates.emplace_back(ops[i].second.first, ops[i].second.second);
            ++ops1;
        }
    }

    assert(0 <= ops1 && ops1 <= 50000);

    sort(updates.begin(), updates.end());
    for (int i = 1; i < int(updates.size()); ++i) {
        using namespace rel_ops;
        assert(updates[i] != updates[i - 1]);
    }

    VectorSum lower(updates.size()), upper(updates.size());

    set<Vector> elements;
    for (int i = 0; i < N; ++i)
        if (ops[i].first == 1) {
            int A, B; tie(A, B) = ops[i].second;
            elements.emplace(A, B);
            int index = lower_bound(updates.begin(), updates.end(), Vector(A, B)) - updates.begin();

            lower.update(index, Vector(A, B));
            upper.update(updates.size() - index - 1, Vector(A, B));
        } else if (ops[i].first == 2) {
            int A, B; tie(A, B) = ops[i].second;
            int index = lower_bound(updates.begin(), updates.end(), Vector(A, B)) - updates.begin();

            assert(elements.count(Vector(A, B)) == 1);
            elements.erase(Vector(A, B));
            lower.update(index, Vector());
            upper.update(updates.size() - index - 1, Vector());
        } else {
            assert(elements.size() > 0);
            int X, Y; tie(X, Y) = ops[i].second;
            auto yMin = lower.getY(X);
            auto yMax = upper.getY(X);

            if (yMin <= Y && Y <= yMax)
                cout << "DA\n";
            else
                cout << "NU\n";
        }
}
