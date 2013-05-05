#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>

using namespace std;

class Node {
  public:
    Node(const int, Node * const, Node * const);

    int sum;
    Node *left, *right;
} *nil = nullptr;

Node::Node(const int _sum, Node * const _left = nil, Node * const _right = nil):
        sum(_sum), left(_left), right(_right) {
}

class SegmentTree {
  public:
    SegmentTree() {
        if (!nil) {
            nil = new Node(0);
            nil -> left = nil -> right = nil;
        }

        root_ = new Node(0);
    }

    SegmentTree(const SegmentTree&) = default;

    void operator=(const SegmentTree&) = delete;

    void add(int position, int value) {
        add(root_, 1, kMaxValue, position, value);
    }

    int query(int position) {
        return query(root_, 1, kMaxValue, position);
    }

    int kth0(int position) {
        return kth0(root_, 1, kMaxValue, position);
    }

  private:
    void add(Node * root, int left, int right, int position, int value) {
        if (left == right) {
            root -> sum += value;
            return;
        }

        int mid = (left + right) / 2;
        if (position <= mid) {
            if (root -> left == nil)
                root -> left = new Node(0);
            add(root -> left, left, mid, position, value);
        } else {
            if (root -> right == nil)
                root -> right = new Node(0);
            add(root -> right, mid + 1, right, position, value);
        }

        root -> sum = root -> left -> sum + root -> right -> sum;
    }

    int query(Node * root, int left, int right, int position) {
        if (root == nil)
            return 0;

        if (right <= position)
            return root -> sum;

        if (position < left)
            return 0;
        int mid = (left + right) / 2;
        return query(root -> left, left, mid, position) + query(root -> right, mid + 1, right, position);
    }

    int kth0(Node * root, int left, int right, int position) {
        if (left == right)
            return left;

        int mid = (left + right) / 2;
        int in_left = (mid - left + 1) - root -> left -> sum;
        if (in_left < position)
            return kth0(root -> right, mid + 1, right, position - in_left);
        return kth0(root -> left, left, mid, position);
    }

    const int kMaxValue = 1000000000;
    Node *root_;
};

class Table {
  public:
    void insert(const string &S, int changeable, int index) {
        if (changeable)
            ids_[S].add(index, 1);

        if (++taken_[S] == 1) {
            auto which = prefix(S);
            if (which.second == 0)
                return;
            not_available_[which.first].add(which.second, 1);
        }
    }

    string query(const string &S, int index) {
        if (taken_[S] == 1)
            return S;

        int the_index = ids_[S].query(index);
        int number = not_available_[S].kth0(the_index);
        return S + "_" + to_string(number);
    }

    void erase(const string &S, int changeable, int index) {
        if (changeable)
            ids_[S].add(index, -1);

        if (--taken_[S] == 0) {
            auto which = prefix(S);
            if (which.second == 0)
                return;
            not_available_[which.first].add(which.second, -1);
        }
    }

  private:
    string to_string(int value) {
        string result;
        do {
            result.push_back(value % 10 + '0');
            value /= 10;
        } while (value);
        reverse(result.begin(), result.end());
        return result;
    }


    pair<string, int> prefix(const string &S) { 
        string result = S;
        int number = 0, last = 0;
        int power = 1;
        for (int i = 1; i <= 8; ++i) {
            if (result.size() == 0)
                break;

            if (result.back() == '_') {
                break;
            }


            if (result.back() < '0' || result.back() > '9') {
                last = 0;
                break;
            }

            last = result.back() - '0';
            result.pop_back();
            number = number + last * power;
            power *= 10;
        }

        if (result != "" and number > 0 and last > 0 and result.back() == '_') {
            result.pop_back();
            return {result, number};
        }

        return {"", 0};
    }

    unordered_map<string, SegmentTree> ids_;
    unordered_map<string, int> taken_;
    unordered_map<string, SegmentTree> not_available_;
};

int main() {
    ifstream cin("input.txt");
    ofstream cout("output.txt");

    int N; cin >> N;

    vector<string> S(N);
    vector<int> changeable(N, 0);

    Table T;
    for (int i = 0; i < N; ++i) {
        cin >> S[i];
        cin >> changeable[i];
        T.insert(S[i], changeable[i], i + 1);
    }

    int M; cin >> M;
    for (int i = 0; i < M; ++i) {
        string type; cin >> type;

        if (type == "?") {
            int which; cin >> which;
            --which;
            if (changeable[which] == 0)
                cout << S[which] << "\n";
            else
                cout << T.query(S[which], which + 1) << "\n";
            continue;
        }

        int which, change;
        string newS;
        cin >> which >> newS >> change;

        --which;
        T.erase(S[which], changeable[which], which + 1);
        S[which] = newS;
        changeable[which] = change;
        T.insert(S[which], changeable[which], which + 1);
    }
}
