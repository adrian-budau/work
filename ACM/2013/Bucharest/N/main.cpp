#include <cstdio>
#include <cstring>
#include <cassert>
#include <algorithm>
#include <vector>
#include <set>

using namespace std;

class fcomp {
  public:
    bool operator()(const pair<int, int> &a, const pair<int, int> &b) const {
        return a.second < b.second;
    }
};

const int maxn = 100005;
int stramosi[maxn][21];

const int maxv = 1000 * 1000 * 1000;

class ARB {
  public:
    ARB():
        left(0), right(0), value(maxv + 1), pos(-1) {
    }

    ARB *left, *right;
    int value, pos;
} *root = new ARB();

void clear(ARB* root) {
    root -> value = maxv + 1;
    root -> pos = -1;
    if (root -> left) {
        clear(root -> left);
        delete root -> left;
        root -> left = 0;
    }
    if (root -> right) {
        clear(root -> right);
        delete root -> right;
        root -> right = 0;
    }
}

void update(ARB* root, int left, int right, int where, int value, int pos) {
    if (value < root -> value) {
        root -> value = value;
        root -> pos = pos;
    }

    if (left == right)
        return;

    int mid = (left + right) / 2;
    if (where <= mid) {
        if (root -> left == 0)
            root -> left = new ARB();

        update(root -> left, left, mid, where, value, pos);
    } else {
        if (root -> right == 0)
            root -> right = new ARB();
        update(root -> right, mid + 1, right, where, value, pos);
    }
}

pair<int, int> query(ARB *root, int left, int right, int start, int finish) {
    if (root == 0) {
        return make_pair(maxv + 1, -1);
    }

    if (start <= left && right <= finish)
        return make_pair(root -> value, root -> pos);

    int mid = (left + right) / 2;
    if (finish <= mid)
        return query(root -> left, left, mid, start, finish);

    if (mid < start)
        return query(root -> right, mid + 1, right, start, finish);

    pair<int, int> v = query(root -> left, left, mid, start, finish);
    pair<int, int> v2 = query(root -> right, mid + 1, right, start, finish);

    if (v.first < v2.first)
        return v;
    return v2;
}

int main() {
    int N, M;

    while (scanf("%d %d", &N, &M) == 2) {
    assert(1 <= N && N <= 100000 && 1 <= M && M <= 100000);

    vector< pair<int, int> > A(N);
    for (int i = 0; i < N; ++i) {
        assert(scanf("%d %d", &A[i].first, &A[i].second) == 2);
        assert(A[i].first < A[i].second && 0 <= A[i].first && A[i].second <= maxv);
    }

    sort(A.begin(), A.end(), fcomp());

    int last = 0;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < 21; ++j)
            stramosi[i][j] = -1;
    clear(root);

    for (int i = 0; i < N; ++i) {
        while (last < i && A[last].second <= A[i].first) {
            stramosi[last][0] = i;
            ++last;
        }

        update(root, 0, maxv, A[i].first, A[i].second, i);
    }

    for (int i = N - 1; i >= 0; --i) {
        for (int j = 1; j < 21; ++j) {
            int x = stramosi[i][j - 1];
            if (x == -1)
                break;
            stramosi[i][j] = stramosi[x][j - 1];
        }
    }

    for (int i = 0; i < M; ++i) {
        int x, y;
        assert(scanf("%d %d", &x, &y) == 2);
        assert(0 <= x && x <= y && y <= maxv);

        int start = query(root, 0, maxv, x, y).second;
        if (start == -1) {
            printf("0\n");
            continue;
        }

        if (A[start].second > y) {
            printf("0\n");
            continue;
        }

        int answer = 1;
        for (int j = 20; j >= 0; --j) {
            if (stramosi[start][j] == -1)
                continue;
            int x = stramosi[start][j];
            if (A[x].second <= y) {
                answer += (1 << j);
                start = x;
            }
        }

        printf("%d\n", answer);
    }
    }
}
