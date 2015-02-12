#include <iostream>
#include <fstream>
#include <memory>
#include <cassert>
#include <vector>
#include <deque>

using namespace std;

class Reader {
  public:
    Reader(const string& filename):
            m_stream(filename),
            m_pos(kBufferSize - 1),
            m_buffer(new char[kBufferSize]) {
        next();
    }

    Reader& operator>>(int& value) {
        value = 0;
        while (current() < '0' || current() > '9')
            next();
        while (current() >= '0' && current() <= '9') {
            value = value * 10 + current() - '0';
            next();
        }
        return *this;
    }

  private:
    const int kBufferSize = 32768;

    char current() {
        return m_buffer[m_pos];
    }

    void next() {
        if (++m_pos == kBufferSize) {
            m_stream.read(m_buffer.get(), kBufferSize);
            m_pos = 0;
        }
    }

    ifstream m_stream;
    int m_pos;
    unique_ptr<char[]> m_buffer;
};

class SegmentTree {
  public:
    SegmentTree() = default;

    SegmentTree add(int value, int many, int from, int to) {
        return SegmentTree(m_root->add(value, many, from, to));
    }

  private:
    struct Node {
        Node *left;
        Node *right;
        int sum;
        Node* add(int value, int many, int from, int to) {
            if (to - from == 1)
                return new Node{left, right, sum + many};

            int mid = (from + to) / 2;
            if (value < mid) {
                if (!left)
                    left = kDefault;
                return new Node{left->add(value, many, from, mid), right, sum + many};
            } else {
                if (!right)
                    right = kDefault;
                return new Node{left, right->add(value, many, mid, to), sum + many};
            }
        }
    };

    SegmentTree(Node* const root):
            m_root(root) {
    }


    Node* m_root = new Node{nullptr, nullptr, 0};
    static Node *kDefault;

    friend int query(SegmentTree, SegmentTree, int, int, int);
};
SegmentTree::Node * SegmentTree::kDefault = new Node{nullptr, nullptr, 0};

int query(SegmentTree _minus, SegmentTree _plus, int from, int to, int k) {
    auto minus = _minus.m_root, plus = _plus.m_root;
    auto leftValue = [&](SegmentTree::Node *node) {
        if (node && node->left)
            return node->left->sum;
        return 0;
    };

    while (to - from > 1) {
        int mid = (from + to) / 2;
        if (leftValue(plus) - leftValue(minus) < k) {
            k -= leftValue(plus) - leftValue(minus);
            from = mid;
            if (plus)
                plus = plus->right;
            if (minus)
                minus = minus->right;
            continue;
        }
        to = mid;
        if (plus)
            plus = plus->left;
        if (minus)
            minus = minus->left;
    }
    return from;
}

int main() {
    Reader cin("kthvalue.in");
    ofstream cout("kthvalue.out");

    int M; cin >> M;

    assert(1 <= M && M <= 1000 * 1000);
    deque<SegmentTree> D(1);
    for (int i = 0; i < M; ++i) {
        int type; cin >> type;
        if (type == 1) {
            int x; cin >> x;
            assert(1 <= x && x <= M);
            D.push_front(D.front().add(x, -1, 0, M + 1));
        } else if (type == 2) {
            int x; cin >> x;
            assert(1 <= x && x <= M);
            D.push_back(D.back().add(x, 1, 0, M + 1));
        } else if (type == 3) {
            assert(!D.empty());
            D.pop_front();
        } else if (type == 4) {
            assert(!D.empty());
            D.pop_back();
        } else {
            int x, y, k; cin >> x >> y >> k;
            assert(1 <= x && x <= y && y <= int(D.size()));
            assert(1 <= k && k <= y - x + 1);

            auto minus = D[x - 1], plus = D[y];
            cout << query(minus, plus, 0, M + 1, k) << "\n";
        }
    }
}
