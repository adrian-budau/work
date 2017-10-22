#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

static const int kMaxV = 1000 * 1000 * 1000 + 3;

struct SegmentTree {
    SegmentTree(int, int) {
        left = nullptr;
        right = nullptr;
    }
    int64_t lazy = 0;
    SegmentTree *left, *right;
};

SegmentTree* add(SegmentTree *node, int begin, int end, int from, int v) {
    if (from <= begin) {
        SegmentTree *x = new SegmentTree(begin, end);
        x->left = node->left;
        x->right = node->right;
        x->lazy = node->lazy + v;
        return x;
    }
    if (from > end)
        return node;

    int mid = (begin + end) / 2;
    SegmentTree *left = node->left, *right = node->right;
    if (left == nullptr)
        left = new SegmentTree(begin, mid);
    if (right == nullptr)
        right = new SegmentTree(mid + 1, end);
    left = add(left, begin, mid, from, v);
    right = add(right, mid + 1, end, from, v);
    SegmentTree *x = new SegmentTree(begin, end);
    x->left = left;
    x->right = right;
    x->lazy = node->lazy;

    return x;
}

int64_t query(SegmentTree *node, int begin, int end, int where ) {
    if (begin == end)
        return node->lazy;
    int mid = (begin + end) / 2;
    if (where <= mid)
        if (node->left == nullptr)
            return node->lazy;
        else
            return node->lazy + query(node->left, begin, mid, where);
    else
        if (node->right == nullptr)
            return node->lazy;
        else
            return node->lazy + query(node->right, mid + 1, end, where);
}

int main() {
    ios::sync_with_stdio(false);
    int N, M; cin >> N >> M;
    vector<int> A(N);
    for (int i = 0; i < N; ++i)
        cin >> A[i];

    SegmentTree *root = new SegmentTree(1, kMaxV);
    vector<SegmentTree*> partial(N);
    for (int i = 0; i < N; ++i) {
        partial[i] = root;
        if (i > 0)
            partial[i] = partial[i - 1];
        partial[i] = add(partial[i], 1, kMaxV, A[i], A[i]);
    }

    for (int i = 0; i < M; ++i) {
        int L, R; cin >> L >> R;
        --L; --R;

        int64_t start = 1;
        SegmentTree *right = partial[R], *left = root;
        if (L > 0)
            left = partial[L - 1];
        while (start <= kMaxV) {
            int64_t have = query(right, 1, kMaxV, start) - query(left, 1, kMaxV, start);
            if (have < start)
                break;
            start = have + 1;
        }
        if (start <= kMaxV)
            cout << start << "\n";
        else
            cout << query(right, 1, kMaxV, kMaxV) - query(left, 1, kMaxV, kMaxV) + 1 << "\n";
    }
}
