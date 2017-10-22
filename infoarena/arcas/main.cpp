#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

struct OP {
    OP(int _type, int _x, int _y, int _v, int _i = 0):
        type(_type), x(_x), y(_y), v(_v), i(_i) {
    }

    bool operator<(const OP& that) const {
        if (y != that.y)
            return y < that.y;
        return type < that.type;
    }

    int type, x, y, v;
    int i;
};

class FenwickTree {
  public:
    FenwickTree(int size):
        m_size(size),
        m_data(size, 0) {
    }

    void add(int x, int v) {
        for (;x < m_size; x += (x + 1) & (-x - 1))
            m_data[x] += v;
    }

    int query(int x, int y) {
        return query(y) - query(x - 1);
    }

    int query(int x) {
        if (x >= m_size)
            x = m_size - 1;
        int sum = 0;
        for (; x >= 0; x -= (x + 1) & (-x - 1))
            sum += m_data[x];
        return sum;
    }

  private:
    int m_size;
    vector<int> m_data;
};

int main() {
    ifstream cin("arcas.in");
    ofstream cout("arcas.out");

    int N, M; cin >> N >> M;

    vector<OP> ops;
    ops.reserve(2 * N + M);
    int maxX = 0;
    for (int i = 0; i < N; ++i) {
        int x, y1, y2; cin >> x >> y1 >> y2;
        assert(1 <= x && x <= 100 * 1000);
        assert(1 <= y1 && y1 <= 100 * 1000);
        assert(1 <= y2 && y2 <= 100 * 1000);
        assert(y1 <= y2);
        if (y1 > y2)
            swap(y1, y2);
        ops.push_back(OP(1, x - 1, y1 - x, 1));
        ops.push_back(OP(3, x - 1, y2 - x, -1));
        maxX = max(maxX, x);
    }

    for (int i = 0; i < M; ++i) {
        int x, y, r; cin >> x >> y >> r;
        assert(1 <= x && x <= 100 * 1000);
        assert(1 <= y && y <= 100 * 1000);
        assert(1 <= r && r <= 100 * 1000);
        ops.push_back(OP(2, x - 1, y - x, x - 1 + r, i));
        maxX = max(maxX, x);
    }

    sort(ops.begin(), ops.end());
    FenwickTree F(maxX);

    vector<int> answer(M);
    for (auto &op : ops) {
        if (op.type != 2)
            F.add(op.x, op.v);
        else
            answer[op.i] = F.query(op.x, op.v);
    }

    for (auto &a : answer)
        cout << a << "\n";
}
