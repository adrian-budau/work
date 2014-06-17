#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <map>

using namespace std;

struct Point {
    int x, y;

    Point rotate90(int many) const {
        Point answer{x, y};
        for (int i = 0; i < many; ++i)
            answer = {-answer.y, answer.x};
        return answer;
    }

    Point operator+(const Point& that) const {
        return {x + that.x, y + that.y};
    }

    Point operator-(const Point& that) const {
        return {x - that.x, y - that.y};
    }

    bool operator<(const Point& that) const {
        if (x != that.x)
            return x < that.x;
        return y < that.y;
    }
};

istream& operator>>(istream& input, Point& object) {
    input >> object.x >> object.y;
    return input;
}

vector<int> check(const vector<Point>& points, const map<Point, int>& index, int rotate, const Point& shift) {
    vector<int> next(points.size(), -1), prev(points.size(), -1);
    for (int i = 0; i < int(points.size()); ++i) {
        auto newpoint = points[i].rotate90(rotate) + shift;

        auto it = index.find(newpoint);
        if (it == index.end())
            continue;
        next[i] = it -> second;
        prev[it -> second] = i;
    }

    // now every chain or path has to be even

    // paths first
    vector<int> answer(points.size(), 0);
    for (int i = 0; i < int(points.size()); ++i)
        if (prev[i] == -1) { // beginning
            int j, type;
            for (j = i, type = 1; j != -1; j = next[j], type = 3 - type)
                answer[j] = type;
            if (type == 2) // odd length path
                return vector<int>();
        }

    // chains now
    for (int i = 0; i < int(points.size()); ++i)
        if (answer[i] == 0) {
            int j, type;
            for (j = i, type = 1; answer[j] == 0; j = next[j], type = 3 - type)
                answer[j] = type;
            if (type == 2) // odd length
                return vector<int>();
        }

    return answer;
}

int main() {
    ifstream cin("overlap.in");
    ofstream cout("overlap.out");

    int N; cin >> N;
    vector<Point> P(N);
    for (int i = 0; i < N; ++i)
        cin >> P[i];

    map<Point, int> index;
    for (int i = 0; i < int(P.size()); ++i)
        index[P[i]] = i;

    for (int i = 0; i < 4; ++i) // choose how many times we rotate
        for (int j = 1; j < N; ++j) { // choose the symmetric 
            auto rotate = P.front().rotate90(i);
            auto shift = P[j] - rotate;

            auto answer = check(P, index, i, shift);
            if (not answer.empty()) {
                for (auto &value : answer)
                    cout << value << "\n";
                return 0;
            }
        }
}
