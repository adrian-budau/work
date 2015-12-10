#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

using namespace std;

int cmp(pair<int, int> A, pair<int, int> B) {
    int ratio = A.first * B.second - A.second * B.first;
    if (ratio > 0)
        return -1;
    if (ratio < 0)
        return 1;
    return 0;
}

int64_t area(pair<int, int> A, pair<int, int> B, pair<int, int> C) {
    return A.first * int64_t(B.second - C.second) + B.first * int64_t(C.second - A.second) + C.first * int64_t(A.second - B.second);
}

class Reader {
  public:
    Reader(istream& stream):
        m_stream(stream),
        m_pos(32767),
        m_data(new char[32768]) {
        next();
    }

    ~Reader() {
        delete m_data;
    }

    template<class T>
    Reader& operator>>(T &X);

    Reader& operator>>(int &X) {
        X = 0;
        while (current() < '0' || current() > '9')
            next();

        while (current() >= '0' && current() <= '9') {
            X = X * 10 + current() - '0';
            next();
        }

        return *this;
    }

  private:
    void next() {
        if (++m_pos == 32768) {
            m_stream.read(m_data, 32768);
            m_pos = 0;
        }
    }

    char current() const {
        return m_data[m_pos];
    }

    istream& m_stream;
    int m_pos;
    char *m_data;
};

int main() {
    ifstream input("combl.in");
    Reader cin(input);
    ofstream cout("combl.out");

    vector< pair<int, int> > lower;
    int N; cin >> N;

    for (int i = 0; i < N; ++i) {
        int type; cin >> type;
        if (type == 1) {
            int a, b; cin >> a >> b;
            bool inserted = false;
            for (auto it = lower.begin(); it != lower.end(); ++it)
                if (cmp(make_pair(a, b), *it) <= 0) {
                    lower.insert(it, make_pair(a, b));
                    inserted = true;
                    break;
                }
            if (not inserted)
                lower.emplace_back(a, b);
        } else if (type == 2) {
            int a, b; cin >> a >> b;
            lower.erase(find(lower.begin(), lower.end(), make_pair(a, b)));
        } else {
            int X, Y; cin >> X >> Y;
            int nowX = 0, nowY = 0;
            vector< pair<int, int> > points;
            for (auto &v : lower) {
                nowX += v.first;
                nowY += v.second;
                points.emplace_back(nowX, nowY);
            }

            for (auto &v: lower) {
                nowX -= v.first;
                nowY -= v.second;
                points.emplace_back(nowX, nowY);
            }

            points.push_back(points[0]);

            bool bad = false;
            for (int i = 1; i < int(points.size()); ++i)
                if (area(points[i - 1], make_pair(X, Y), points[i]) > 0) {
                    bad = true;
                    break;
                }

            if (bad)
                cout << "NU\n";
            else
                cout << "DA\n";
        }
    }
}
