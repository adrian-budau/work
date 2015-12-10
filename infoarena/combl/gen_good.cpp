#include <vector>
#include <algorithm>
#include <fstream>
#include <inputGenerator.hpp>
#include <string>
#include <list>

using namespace std;
using namespace inputGenerator;

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


int main(int argc, char* argv[]) {
    vector<string> arguments(argv, argv + argc);

    if (arguments.size() < 4) {
        cerr << "Generator error: Expecting 3 arguments N Q MAXV\n";
        return 1;
    }

    int N = stoi(arguments[1]), Q = stoi(arguments[2]), MAXV = stoi(arguments[3]);
    cout << Q << "\n";
    Seed::logging = false;
    if (arguments.size() > 4)
        reSeed(stoul(arguments[4]));

    auto points = randomSample(N, 0, MAXV * MAXV - 1);
    vector<pair<int, int>> realPoints;
    for (int i = 0; i < N; ++i)
        realPoints.emplace_back(points[i] / MAXV + 1, points[i] % MAXV + 1);
    sort(realPoints.begin(), realPoints.end(), [&](pair<int, int> A, pair<int, int> B) {
        return A.first * B.second > A.second * B.first;
    });

    int deletes = randomInt(0, min(N / 2, max(Q - N - 1, 0)));

    vector<int> ops(Q);
    for (int i = 0; i < N; ++i)
        ops[i] = 1;
    for (int i = N; i < N + deletes; ++i)
        ops[i] = 2;
    for (int i = N + deletes; i < Q; ++i)
        ops[i] = 3;

    randomShuffle(ops.begin() + N / 2 + 1, ops.end() - 1);

    vector<int> pointsLeft(N);
    for (int i = 0; i < N; ++i)
        pointsLeft[i] = i;
    randomShuffle(pointsLeft.begin(), pointsLeft.end());
    vector<int> now;

    VectorSum lower(N), upper(N);
    int sumX = 0;
    for (auto &op: ops) {
        cout << op << " ";
        if (op == 1) {
            now.push_back(pointsLeft.back());
            int X, Y; tie(X, Y) = realPoints[pointsLeft.back()];
            sumX += X;
            cout << X << " " << Y << "\n";
            lower.update(pointsLeft.back(), Vector(X, Y));
            upper.update(N - pointsLeft.back() - 1, Vector(X, Y));
            pointsLeft.pop_back();
        } else if (op == 2) {
            auto pos = randomUInt(0, now.size() - 1);
            int X, Y; tie(X, Y) = realPoints[now[pos]];
            sumX -= X;
            lower.update(now[pos], Vector());
            upper.update(N - now[pos] - 1, Vector());
            cout << X << " " << Y << "\n";
            swap(now[pos], now.back());
            now.pop_back();
        } else {
            int X = randomInt(0, min(1000000000, sumX + MAXV)); // to get a 1 / N chance to be too much to the right
            auto yMin = lower.getY(X);
            auto yMax = lower.getY(X);

            list<int> possibleY{int(yMin.num / yMin.den), int(yMin.num / yMin.den + 1), int(yMax.num / yMax.den), int(yMax.num / yMax.den + 1)};

            possibleY.remove_if([](int Y) {
                return Y < 0 || Y > 1000000000;
            });

            cout << X << " " << randomElement(possibleY) << "\n";
        }
    }
}
