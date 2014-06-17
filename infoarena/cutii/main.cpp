#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

class FenwickTree2D {
  public:
    FenwickTree2D(int size_x, int size_y):
            size_x_(size_x),
            size_y_(size_y),
            data_(size_x + 1, vector<int>(size_y + 1, 0)) {
    }

    template<class Function>
    void update(int X, int Y, Function f) {
        for (int x = X; x <= size_x_; x += x & -x)
            for (int y = Y; y <= size_y_; y += y & -y)
                f(data_[x][y]);
    }

    template<class Function>
    void query(int X, int Y, Function f) const {
        for (int x = X; x; x -= x & -x)
            for (int y = Y; y; y -= y & -y)
                f(data_[x][y]);
    }

  private:
    int size_x_, size_y_;
    vector< vector<int> > data_;
};

class Box {
  public:
    Box(int _x = 0, int _y = 0, int _z = 0):
            x(_x), y(_y), z(_z) {
    }

    bool operator<(const Box& that) const {
        if (x != that.x)
            return x < that.x;
        if (y != that.y)
            return y < that.y;
        return z < that.z;
    }

    int x, y, z;
};

istream& operator>>(istream& input, Box& object) {
    input >> object.x >> object.y >> object.z;
    return input;
}

int main() {
    ifstream cin("cutii.in");
    ofstream cout("cutii.out");

    int N, T; cin >> N >> T;

    FenwickTree2D F(N, N);
    while (T--) {
        vector<Box> Boxes(N);
        for (int i = 0; i < N; ++i)
            cin >> Boxes[i];

        sort(Boxes.begin(), Boxes.end());

        int answer = 0;
        for (auto &box : Boxes) {
            int current = 0;
            F.query(box.y - 1, box.z - 1, [&](int value) {
                current = max(current, value);
            });
            ++current;
            answer = max(answer, current);

            F.update(box.y, box.z, [&](int &value) {
                value = max(value, current);
            });
        }

        cout << answer << "\n";

        for (auto &box : Boxes)
            F.update(box.y, box.z, [](int &value) {
                value = 0;
            });
    }
}
