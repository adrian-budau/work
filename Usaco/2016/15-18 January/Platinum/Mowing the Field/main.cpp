#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

static const int kMaxValue = 200 * 1000;

struct Node {
    int value;
    Node *left, *right;
};


static const int kAmount = 1000000;
Node blockNode[19 * kAmount];

int pNode = 0, pNode2D = 0;

class SegmentTree {
  public:
    SegmentTree(int size = 0) {
        if (size != 0) {
            m_root = &blockNode[pNode++];
            m_size = size;
        }
    }

    void add(int from, int to, int value) {
        add(m_root, 0, m_size, from, to, value);
    }

    int query(int where) {
        return query(m_root, 0, m_size, where);
    }

  private:
    void add(Node *node, int begin, int end, int from, int to, int value) {
        if (from <= begin && end <= to) {
            node->value += value;
            return;
        }

        int mid = (begin + end) / 2;
        if (from <= mid) {
            if (!node->left)
                node->left = &blockNode[pNode++];
            add(node->left, begin, mid, from, to, value);
        }
        if (mid < to) {
            if (!node->right)
                node->right = &blockNode[pNode++];
            add(node->right, mid + 1, end, from, to, value);
        }
    }

    int query(Node* node, int begin, int end, int where) {
        if (node == nullptr)
            return 0;
        if (begin == end)
            return node->value;
        int mid = (begin + end) / 2;
        if (where <= mid)
            return node->value + query(node->left, begin, mid, where);
        else
            return node->value + query(node->right, mid + 1, end, where);
    }

    Node *m_root;
    int m_size;
};

struct Node2D {
    SegmentTree tree;
    Node2D *left, *right;
};
Node2D blockNode2D[kAmount];

class SegmentTree2D {
  public:
    SegmentTree2D(int size, int second_size):
        m_root(&blockNode2D[pNode2D++]),
        m_size(size), m_second_size(second_size) {
            m_root->tree = SegmentTree(second_size);
    }

    void add(int ox, int from, int to, int value) {
        if (from > to)
            swap(from, to);
        if (to - from < 2)
            return;
        add(m_root, 0, m_size, ox, from + 1, to - 1, value);
    }

    int query(int oy, int from, int to) {
        if (from > to)
            swap(from, to);
        if (to - from < 2)
            return 0;
        return query(m_root, 0, m_size, oy, from + 1, to - 1);
    }

  private:
    void add(Node2D* node, int begin, int end, int ox, int from, int to, int value) {
        node->tree.add(from, to, value);
        if (begin == end)
            return;
        int mid = (begin + end) / 2;
        if (ox <= mid) {
            if (!node->left) {
                node->left = &blockNode2D[pNode2D++];
                node->left->tree = SegmentTree(m_second_size);
            }
            add(node->left, begin, mid, ox, from, to, value);
        } else {
            if (!node->right) {
                node->right = &blockNode2D[pNode2D++];
                node->right->tree = SegmentTree(m_second_size);
            }
            add(node->right, mid + 1, end, ox, from, to, value);
        }
    }

    int query(Node2D* node, int begin, int end, int oy, int from, int to) {
        if (node == nullptr)
            return 0;
        if (from <= begin && end <= to)
            return node->tree.query(oy);

        int mid = (begin + end) / 2;
        int answer = 0;
        if (from <= mid)
            answer += query(node->left, begin, mid, oy, from, to);
        if (mid < to)
            answer += query(node->right, mid + 1, end, oy, from, to);
        return answer;
    }

    Node2D* m_root;
    int m_size, m_second_size;
};

int main() {
    ifstream cin("mowing.in");
    ofstream cout("mowing.out");

    int N, T; cin >> N >> T;
    SegmentTree2D OX(kMaxValue, kMaxValue), OY(kMaxValue, kMaxValue);
    vector< pair<int, int> > P(N);

    vector<int> ox(N), oy(N);
    for (int i = 0; i < N; ++i) {
        cin >> P[i].first >> P[i].second;
        ox[i] = P[i].first;
        oy[i] = P[i].second;
    }

    sort(ox.begin(), ox.end());
    sort(oy.begin(), oy.end());
    ox.erase(unique(ox.begin(), ox.end()), ox.end());
    oy.erase(unique(oy.begin(), oy.end()), oy.end());
    for (int i = 0; i < N; ++i) {
        P[i].first = lower_bound(ox.begin(), ox.end(), P[i].first) - ox.begin();
        P[i].second = lower_bound(oy.begin(), oy.end(), P[i].second) - oy.begin();
    }

    int64_t answer = 0;
    for (int i = 1; i < N; ++i) {
        if (i - T > 0) {
            if (P[i - T].first == P[i - T - 1].first)
                OX.add(P[i - T].first, P[i - T].second, P[i - T - 1].second, 1);
            else
                OY.add(P[i - T].second, P[i - T].first, P[i - T - 1].first, 1);
        }

        if (P[i].first == P[i - 1].first) {
            answer += OY.query(P[i].first, P[i].second, P[i - 1].second);
        } else {
            answer += OX.query(P[i].second, P[i].first, P[i - 1].first);
        }
    }

    cout << answer << "\n";
}
