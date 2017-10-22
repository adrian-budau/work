#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

struct Treap {
    Treap(int64_t _sum):
        subtree_size(1),
        sum(_sum),
        left(nullptr),
        right(nullptr) {
    }

    void update() {
        subtree_size = 0;
        sum = 0;
        if (left != nullptr) {
            subtree_size += left->subtree_size;
            sum += left->sum;
        }
        if (right != nullptr) {
            subtree_size += right->subtree_size;
            sum += right->sum;
        }
    }

    int subtree_size;
    int64_t sum;
    Treap *left, *right;
};

int query(Treap* node, int64_t sum) {
    int answer = 0;
    if (node->left != nullptr) {
        if (node->left->sum < sum) {
            sum -= node->left->sum;
            answer += node->left->subtree_size;
        } else
            return query(node->left, sum);
    }
    if (node->sum < sum) {
        sum -= node->sum;
        ++answer;
        return answer + query(node->right, sum);
    }
    return answer;
}

pair<Treap*, Treap*> split(Treap* root, int index) {
    int left_size = 0;
    if (root->left != nullptr)
        left_size = root->left->subtree_size;
    root->left = 
}

Treap* merge(Treap* root, Treap* node, int index) {
    if (root == nullptr)
        return node;
    if (rand() % (root->subtree_size + 1) == 0) {
        Treap *left, *right;
        tie(left, right) = split(root, index);
        node->left = left;
        node->right = right;
        node->update();
        return node;
    }

    int left_size = 0;
    if (root->left != nullptr)
        left_size = root->left->subtree_size;
    if (left_size + 1 >= index) {
        root->left = merge(root->left, node, index);
        root->update();
        return root;
    }
    index -= left_size + 1;
    root->right = merge(root->right, node, index);
    root->update();
    return root;
}

int main() {
    ifstream cin("input");
    ofstream cout("output");

    int N; cin >> N;
    srand(time(NULL));
    Treap* root = new Treap(0);
    vector<int> larger_than(N, 0);
    for (int i = 0; i < N; ++i) {
        int64_t P; cin >> P;
        int64_t aux = P;
        --aux;
        larger_than[i] = query(root, aux) + 1;
        Treap* node = new Treap(P);
        root = merge(root, node, larger_than[i]);
    }
}
