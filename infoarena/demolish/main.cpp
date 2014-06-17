#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <cassert>

using namespace std;

struct Rectangle {
    int x1, y1, x2, y2, cost;
};

istream& operator>>(istream& input, Rectangle& object) {
    input >> object.x1 >> object.y1 >> object.x2 >> object.y2 >> object.cost;
    return input;
}

ostream& operator<<(ostream& output, const Rectangle& object) {
    output << object.x1 << " " << object.y1 << " " << object.x2 << " " << object.y2 << "\n";
    return output;
}

struct Event : public Rectangle {
    enum Type {
        insert = 0,
        erase = 1
    };

    Event():
            Rectangle(),
            type(Type::insert) {
    }

    Event(const Rectangle& object, Type type_):
            Rectangle(object),
            type(type_) {
    }

    bool isInsert() const {
        return type == Type::insert;
    }

    bool isErase() const {
        return type == Type::erase;
    }

    static Event Insert(const Rectangle& object) {
        return Event(object, Type::insert);
    }

    static Event Erase(const Rectangle& object) {
        return Event(object, Type::erase);
    }

    int type;
};

class SegmentTree {
  public:
    SegmentTree(int size):
            size_(size) {
        int real_size = 1;
        int left = 0, right = size;
        while (left + 1 != right) {
            left = (left + right) / 2;
            real_size *= 2;
            ++real_size;
        }

        ++real_size;
        data_ = vector<Node>(real_size, {0, 0, 0});
        build(1, 0, size);
    }

    void addCost(int from, int to, int cost) {
        if (from >= size_ or to < 0)
            return;
        addCost(1, 0, size_, from, to, cost);
    }

    long long minCost() const {
        return data_[1].cost;
    }

    long long minWhere() const {
        return data_[1].where;
    }

  private:
    struct Node {
        long long cost, lazy;
        int where;

        bool operator<(const Node& that) const {
            if (cost != that.cost)
                return cost < that.cost;
            return where < that.where;
        }
    };

    void build(int node, int left, int right) {
        if (left + 1 == right) {
            data_[node].where = left;
            return;
        }

        int mid = (left + right) / 2;
        build(node * 2, left, mid);
        build(node * 2 + 1, mid, right);
        data_[node].where = data_[node * 2].where;
    }

    void addCost(int node, int left, int right, int from, int to, int cost) {
        if (from <= left and right <= to) {
            data_[node].cost += cost;
            data_[node].lazy += cost;
            return;
        }

        int mid = (left + right) / 2;
        if (from < mid)
            addCost(node * 2, left, mid, from, to, cost);
        if (mid < to)
            addCost(node * 2 + 1, mid, right, from, to, cost);

        Node mint = min(data_[node * 2], data_[node * 2 + 1]);
        data_[node].where = mint.where;
        data_[node].cost = data_[node].lazy + mint.cost;
    }

    int size_;
    vector<Node> data_;
};

int main() {
    ifstream cin("demolish.in");
    ofstream cout("demolish.out");

    int N, M, F, DX, DY; cin >> N >> M >> F >> DX >> DY;

    assert(4 <= N and N <= 500000);
    assert(4 <= M and M <= 500000);
    assert(0 <= F and F <= 30000);
    assert(0 < DX and DX <= N);
    assert(0 < DY and DY < M);
    vector<Rectangle> A(F);
    for (int i = 0; i < F; ++i) {
        cin >> A[i];
        //assert(0 <= A[i].x1 and A[i].x1 < A[i].x2 and A[i].x2 <= N);
        // test 2 is broken
        //assert(0 <= A[i].y1 and A[i].y1 < A[i].y2 and A[i].y2 <= M);
        // test 6 is broken
    }

    vector< vector<Event> > sweep(N + 1);
    for (int i = 0; i < F; ++i) {
        sweep[A[i].x1 + 1].push_back(Event::Insert(A[i]));
        if (A[i].x2 + DX <= N)
            sweep[A[i].x2 + DX].push_back(Event::Erase(A[i]));
    }

    SegmentTree T(M - DY + 1);

    long long answer = numeric_limits<long long>::max();
    Rectangle where;
    for (int i = 0; i <= N; ++i) {
        for (auto &op : sweep[i])
            if (op.isInsert())
                T.addCost(max(op.y1 - DY + 1, 0), op.y2, op.cost);
            else if (op.isErase())
                T.addCost(max(op.y1 - DY + 1, 0), op.y2, -op.cost);
        if (i >= DX and (sweep[i].size() or i == DX)) { // if something did change
            long long now = T.minCost();
            if (now < answer) {
                answer = now;
                int wherenow = T.minWhere();
                where = {i - DX, wherenow, i, wherenow + DY, 0};
            }
        }
    }

    cout << answer << "\n";
    cout << where << "\n";
}
