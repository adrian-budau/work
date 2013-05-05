#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <tuple>

using namespace std;

class FenwickTree {
  public:
    FenwickTree(const int &size, const int &value = 0):
            data_(size + 1) {
        for (int i = 1; i <= size; ++i)
            data_[i] = (i & - i) * value;
    }

    void update(int where, int value) {
        for (;where < int(data_.size()); where += (where & -where))
            data_[where] += value;
    }

    int query(int where) {
        int sum = 0;
        for (; where; where -= (where & -where))
            sum += data_[where];
        return sum;
    }

    int find(int value) {
        int step, i;
        for (step = 1; step < int(data_.size()); step <<= 1);
        for (i = 0; step; step >>= 1)
            if (i + step < int(data_.size()) && data_[i + step] <= value) {
                i += step;
                value -= data_[i];
            }

        if (value)
            return data_.size();
        return i;
    }

  private:
    vector<int> data_;
};

class Node {
  public:
    Node(int, int);

    int value, priority, count;
    Node *left, *right, *root;
} *NIL;

Node::Node(int _value, int _priority):
        value(_value),
        priority(_priority),
        count(1),
        left(NIL),
        right(NIL),
        root(NIL) {
}

Node* root(Node* current) {
    if (current -> root == NIL)
        return current;
    return root(current -> root);
}

void update(Node *current) {
    current -> count = current -> left -> count + current -> right -> count + 1;
}

pair<Node*, Node*> split(Node *current, int value) {
    if (current == NIL)
        return {NIL, NIL};
    if (value <= current -> value) {
        auto left = split(current -> left, value);
        left.first -> root = NIL;

        if (left.second != NIL)
            left.second -> root = current;
        current -> left = left.second;

        update(current);
        return {left.first, current};
    }

    auto right = split(current -> right, value);
    if (right.first != NIL)
        right.first -> root = current;
    right.second -> root = NIL;

    current -> right = right.first;
    update(current);

    return {current, right.second};
}

Node* insert(Node *current, Node* element) {
    if (element -> priority > current -> priority) {
        auto sons = split(current, element -> value);
        element -> left = sons.first;
        element -> right = sons.second;
        if (sons.first != NIL)
            element -> left -> root = element;
        if (sons.second != NIL)
            element -> right -> root = element;
        update(element);
        return element;
    }

    if (element -> value < current -> value) {
        current -> left = insert(current -> left, element);
        current -> left -> root = current;
        update(current);
        return current;
    }

    current -> right = insert(current -> right, element);
    current -> right -> root = current;
    update(current);
    return current;
}

Node* merge(Node *left, Node *right) {
    if (left == NIL and right == NIL)
        return NIL;

    if (left -> priority > right -> priority) {
        left -> right = merge(left -> right, right);
        if (left -> right != NIL)
            left -> right -> root = left;
        update(left);
        return left;
    }

    right -> left = merge(left, right -> left);
    if (right -> left != NIL)
        right -> left -> root = right;
    update(right);
    return right;
}

int main() {
    ifstream cin("input.txt");
    ofstream cout("output.txt");

    int N, M; cin >> N >> M;

    srand(time(NULL));
    NIL = new Node(0, -1);
    NIL -> left = NIL -> right = NIL -> root = NIL;
    NIL -> count = 0;

    vector<Node*> T(N + 1);
    for (int i = 1; i <= N; ++i)
        T[i] = new Node(i, rand());

    FenwickTree F(N, 1);
    long long answer = 0;
    for (int i = 1; i <= M; ++i) {
        int from, to; cin >> from >> to;

        Node* now, *left, *right;
        now = root(T[from]);
        tie(left, now) = split(now, from);
        tie(now, right) = split(now, to + 1);

        merge(left, right);
        if (F.query(now -> value) - F.query(now -> value - 1)) {
            F.update(now -> value, -1);
        }

        // find those with 1
        int before = F.query(from - 1);
        do {
            int next = F.find(before) + 1;
            if (next > to)
                break;
            now = insert(now, T[next]);
            F.update(next, -1);
        } while (true);
        answer += now -> count;
    }

    cout << answer << "\n";
}
