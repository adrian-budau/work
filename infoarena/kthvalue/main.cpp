#include <iostream>
#include <fstream>
#include <memory>
#include <cassert>
#include <vector>

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
    SegmentTree(int value):
            m_value(value) {
    }

    SegmentTree add(int value, int from, int to) {
        return SegmentTree(m_root->add(value, from, to), value);
    }

    int value() const {
        return m_value;
    }

  private:
    struct Node {
        Node *left;
        Node *right;
        int sum;
        Node* add(int value, int from, int to) {
            if (to - from == 1)
                return new Node{left, right, sum + 1};

            int mid = (from + to) / 2;
            if (value < mid) {
                if (!left)
                    left = kDefault;
                return new Node{left->add(value, from, mid), right, sum + 1};
            } else {
                if (!right)
                    right = kDefault;
                return new Node{left, right->add(value, mid, to), sum + 1};
            }
        }
    };

    SegmentTree(Node* const root, int value):
            m_root(root),
            m_value(value) {
    }


    Node* m_root = new Node{nullptr, nullptr, 0};
    static Node *kDefault;
    int m_value;

    friend class Deque;
};
SegmentTree::Node * SegmentTree::kDefault = new Node{nullptr, nullptr, 0};

class Deque {
  public:
    Deque(int treeSize):
            m_tree_size(treeSize) {
    }

    void pushFront(int value) {
        push(m_left, value);
    }

    void pushBack(int value) {
        push(m_right, value);
    }

    void popFront() {
        if (m_left.empty())
            balance(m_left, m_right);
        else
            m_left.pop_back();
    }

    void popBack() {
        if (m_right.empty())
            balance(m_right, m_left);
        else
            m_right.pop_back();
    }

    int size() const {
        return m_left.size() + m_right.size();
    }

    bool empty() const {
        return m_left.empty() && m_right.empty();
    }

    int query(int from, int to, int kth) {
        int left = m_left.size();
        if (from >= left) { // it's only on the right side
            return query(m_right[to - left - 1], 1, from > left ? m_right[from - left - 1] : SegmentTree(0), -1, kth);
        } else if (to <= left) { // it's only on the left side
            return query(m_left[left - from - 1], 1, to < left ? m_left[left - to - 1] : SegmentTree(0), -1, kth);
        } else {
            return query(m_left[left - from - 1], 1, m_right[to - left - 1], 1, kth);
        }
    }

  private:
    void push(vector<SegmentTree>& stack, int value) {
        SegmentTree S(value);

        if (!stack.empty())
            S = stack.back();
        stack.push_back(S.add(value, 0, m_tree_size));
    }

    void balance(vector<SegmentTree>& empty, vector<SegmentTree>& full) {
        vector<int> values(full.size() - 1);
        for (int i = 1; i < int(full.size()); ++i)
            values[i - 1] = full[i].value();

        full.clear();
        for (int i = values.size() / 2; i < int(values.size()); ++i)
            push(full, values[i]);
        for (int i = values.size() / 2 - 1; i >= 0; --i)
            push(empty, values[i]);
    }

    int query(SegmentTree first, int firstMultiplier, SegmentTree second, int secondMultiplier, int kth) {
        SegmentTree::Node *firstNode = first.m_root, *secondNode = second.m_root;

        int left = 0, right = m_tree_size;
        int total = 0;

        auto leftValue = [&](SegmentTree::Node *node) {
            if (node && node->left)
                return node->left->sum;
            return 0;
        };

        while (right - left > 1) {
            int mid = (left + right) / 2;
            if (total + leftValue(firstNode) * firstMultiplier + leftValue(secondNode)  * secondMultiplier < kth) {
                total += leftValue(firstNode) * firstMultiplier + leftValue(secondNode) * secondMultiplier;
                left = mid;
                if (firstNode)
                    firstNode = firstNode->right;
                if (secondNode)
                    secondNode = secondNode->right;
            } else {
                right = mid;
                if (firstNode)
                    firstNode = firstNode->left;
                if (secondNode)
                    secondNode = secondNode->left;
            }
        }
        return left;
    }

    int m_tree_size;
    vector<SegmentTree> m_left, m_right;
};

int main() {
    Reader cin("kthvalue.in");
    ofstream cout("kthvalue.out");

    int M; cin >> M;

    assert(1 <= M && M <= 1000 * 1000);
    Deque D(M + 1);
    for (int i = 0; i < M; ++i) {
        int type; cin >> type;
        if (type == 1) {
            int x; cin >> x;
            assert(1 <= x && x <= M);
            D.pushFront(x);
        } else if (type == 2) {
            int x; cin >> x;
            assert(1 <= x && x <= M);
            D.pushBack(x);
        } else if (type == 3) {
            assert(!D.empty());
            D.popFront();
        } else if (type == 4) {
            assert(!D.empty());
            D.popBack();
        } else {
            int x, y, k; cin >> x >> y >> k;
            assert(1 <= x && x <= y && y <= D.size());
            assert(1 <= k && k <= y - x + 1);
            cout << D.query(x - 1, y, k) << "\n";
        }
    }
}
