#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <set>

using namespace std;

struct fraction {
    fraction(int _num = 0, int _den = 0):
            num(_num),
            den(_den) {
        if (den < 0) {
            num *= -1;
            den *= -1;
        }

        if (den == 0) {
            den = 1;
            num = 1000005;
        }
    }

    bool operator<(const fraction& that) const {
        return num * that.den < den * that.num;
    }

    bool operator==(const fraction& that) const {
        return num * that.den == den * that.num;
    }

    int num, den;
};

struct operation {
    operation(const fraction &_when, int _x, int _y):
            when(_when),
            x(_x),
            y(_y) {
    }

    bool operator<(const operation& that) const {
        return when < that.when;
    }

    bool operator==(const operation& that) const {
        return when == that.when;
    }

    fraction when;
    int x, y;
};

int square_distance(const pair<int, int> &A, const pair<int, int> &B) {
    return (A.first - B.first) * (A.first - B.first) + (A.second - B.second) * (A.second - B.second);
}

class Solver {
  public:
    Solver(int size):
            m_info(size),
            m_count(0),
            m_best(0) {
    }

    void add(int where, int value) {
        if (m_info[where].size()) {
            m_best -= sqrt(*m_info[where].begin());
            --m_count;
        }

        m_info[where].insert(value);
        m_best += sqrt(*m_info[where].begin());
        ++m_count;
    }

    void remove(int where, int value) {
        m_best -= sqrt(*m_info[where].begin());
        m_info[where].erase(m_info[where].find(value));
        --m_count;
        if (m_info[where].size()) {
            m_best += sqrt(*m_info[where].begin());
            ++m_count;
        }
    }

    double best() const {
        if (m_count == int(m_info.size()) - 1)
            return m_best;
        return 1e50;
    }

  private:
    vector< multiset<int> > m_info;
    int m_count;
    double m_best;
};

int main() {
    int N; cin >> N;


    vector< pair<int, int> > points(N);
    for (int i = 0; i < N; ++i)
        cin >> points[i].first >> points[i].second;

    sort(points.begin(), points.end());
    vector< vector<int> > dist(N, vector<int>(N));
    for (int i = 0; i < N; ++i)
        for (int j = i + 1; j < N; ++j) {
            dist[i][j] = square_distance(points[i], points[j]);
            dist[j][i] = dist[i][j];
        }

    Solver S(N);
    for (int i = 0; i < N; ++i)
        for (int j = i + 1; j < N; ++j)
            S.add(j, dist[i][j]);

    vector<operation> D;

    for (int i = 0; i < N; ++i)
        for (int j = i + 1; j < N; ++j) {
            fraction when(points[j].second - points[i].second, points[j].first - points[i].first);
            D.push_back(operation(when, i, j));
        }

    sort(D.begin(), D.end());
    int M = D.size();
    for (int i = 0; i < M; ++i)
        D.push_back(operation(D[i].when, D[i].y, D[i].x));

    double answer = S.best();
    for (int i = 0, j; i < int(D.size()); i = j) {
        for (j = i; j < int(D.size()) && D[j].when == D[i].when; ++j) {
            int d = dist[D[j].x][D[j].y];
            S.remove(D[j].y, d);
            S.add(D[j].x, d);
        }
        answer = min(answer, S.best());
    }

    cout.setf(ios::floatfield, ios::fixed);
    cout.precision(4);
    cout << answer << "\n";
}
