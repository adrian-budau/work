#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

#define SZ(x) ((int) (x).size())
#define SCORE 10
#define OP_LIMIT (n * 10)
#define RANGE_LIMIT 1000000000
using namespace std;

const int NONE = 1e9 + 5;

struct Node {
    int min, max, lazySet, lazyAdd;
};

void result(const string& message, int score) {
    cerr << message;
    cout << score;
    exit(0);
}

class SegmentTree {
public:
    SegmentTree(const vector<int>& values):
        n(SZ(values)),
        tree(vector<Node>(max(5, SZ(values) * 4))) {
            build(0, 0, n - 1, values);
        }

    void update(int x, int value) {
        updateLeft(0, 0, n - 1, x, value);
        updateRight(0, 0, n - 1, x, value);
    }

    vector<int> getNumbers() {
        vector<int> ret;
        getNumbers(0, 0, n - 1, ret);
        return ret;
    }
private:
    vector<Node> tree;
    int n;

    void updateNode(int node) {
        tree[node].min = min(tree[2 * node + 1].min,
                             tree[2 * node + 2].min);
        tree[node].max = max(tree[2 * node + 1].max,
                             tree[2 * node + 2].max);
    }

    void build(int node, int left, int right, const vector<int>& values) {
        tree[node].lazySet = NONE;
        tree[node].lazyAdd = 0;
        if (left == right) {
            tree[node].min = values[left];
            tree[node].max = values[left];
        } else {
            int mid = (left + right) / 2;
            build(2 * node + 1, left, mid, values);
            build(2 * node + 2, mid + 1, right, values);
            updateNode(node);
        }
    }

    void split(int node, int left, int right) {
        if (tree[node].lazySet != NONE) {
            int x = tree[node].lazySet;
            tree[node].max = x;
            tree[node].min = x;
            if (left < right) {
                tree[2 * node + 1].lazySet = x;
                tree[2 * node + 1].lazyAdd = 0;
                tree[2 * node + 2].lazySet = x;
                tree[2 * node + 2].lazyAdd = 0;
            }
            tree[node].lazySet = NONE;
        }
        if (tree[node].lazyAdd != 0) {
            int x = tree[node].lazyAdd;
            tree[node].max += x;
            tree[node].min += x;
            if (left < right) {
                tree[2 * node + 1].lazyAdd += x;
                tree[2 * node + 2].lazyAdd += x;
            }
            tree[node].lazyAdd = 0;
        }
    }

    void updateLeft(int node, int left, int right, int x, int value) {
        split(node, left, right);
        if (tree[node].min >= x) {
            return;
        }
        if (tree[node].max <= x - value) {
            tree[node].lazyAdd += value;
            return;
        }
        if (tree[node].min > x - value && tree[node].max <= x) {
            tree[node].lazySet = x;
            return;
        }
        if (left == right) {
            return;
        }
        int mid = (left + right) / 2;
        updateLeft(2 * node + 1, left, mid, x, value);
        updateLeft(2 * node + 2, mid + 1, right, x, value);
        split(2 * node + 1, left, mid);
        split(2 * node + 2, mid + 1, right);
        updateNode(node);
    }

    void updateRight(int node, int left, int right, int x, int value) {
        split(node, left, right);
        if (tree[node].max <= x) {
            return;
        }
        if (tree[node].min >= x + value) {
            tree[node].lazyAdd -= value;
            return;
        }
        if (tree[node].max < x + value && tree[node].min >= x) {
            tree[node].lazySet = x;
            return;
        }
        if (left == right) {
            return;
        }
        int mid = (left + right) / 2;
        updateRight(2 * node + 1, left, mid, x, value);
        updateRight(2 * node + 2, mid + 1, right, x, value);
        split(2 * node + 1, left, mid);
        split(2 * node + 2, mid + 1, right);
        updateNode(node);
    }

    void getNumbers(int node, int left, int right, vector<int>& a) {
        split(node, left, right);
        if (left == right) {
            a.push_back(tree[node].max);
        } else {
            int mid = (left + right) / 2;
            getNumbers(2 * node + 1, left, mid, a);
            getNumbers(2 * node + 2, mid + 1, right, a);
        }
    }
};

void eprint(const vector<int>& a) {
    for (int x: a) {
        cerr << x << ' ';
    }
    cerr << endl;
}

int main() {
    ifstream fin("magnet.in");
    ifstream fout("magnet.out");
    ifstream fok("magnet.ok");

    int T;
    fin >> T;

    while (T--) {
        int k, rk;
        if (!(fout >> k)) {
            result("Fisier de iesire incomplet", 0);
        }
        fok >> rk;

        int n;
        fin >> n;

        vector<int> init(n);
        for (int i = 0; i < n; ++i) {
            fin >> init[i];
        }

        vector<int> final(n);
        for (int i = 0; i < n; ++i) {
            fin >> final[i];
        }
        sort(init.begin(), init.end());
        sort(final.begin(), final.end());

        if (rk == -1) {
            if (k != -1) {
                result("Raspuns gresit!", 0);
            } else {
                continue;
            }
        }

        if (k < 0) {
            result("Raspuns gresit!", 0);
        }
        if (k > OP_LIMIT) {
            result("Prea multe operatii", 0);
        }

        SegmentTree t(init);
        //eprint(t.getNumbers());
        while (k-- > 0) {
            int x, value;
            if (!(fout >> x >> value)) {
                result("Raspuns gresit!", 0);
            }
            if (x < 0 || x > RANGE_LIMIT) {
                result("Operatie invalida", 0);
            }
            if (value < 0 || value > RANGE_LIMIT) {
                result("Operatie invalida", 0);
            }
            if (value != 0) {
                t.update(x, value);
            }
            //cerr << "OP " << x << ' ' << value << ": ";
            //eprint(t.getNumbers());
        }
        if (t.getNumbers() != final) {
            result("Configuratia finala incorecta", 0);
        }

        while (rk--) {
            int x, value;
            fok >> x >> value;
        }
    }

    result("OK!", SCORE);

    fin.close();
    fout.close();
    fok.close();
}

