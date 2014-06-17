#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

const int kModulo = 1000000007;

class SegmentTree {
  public:
    SegmentTree(const int &size = 1):
        size_(smallestPowerOf2BiggerThanOrEqual(size)),
        sum_(size_ * 2, 0),
        lazy_(size_ * 2, 0) {
    }

    void add(int from, int to, int value) {
        cerr << from << " " << to << " with " << value << "\n";
        add(1, 0, size_, from, to + 1, value);
        for (int i = 0; i < 2 * size_; ++i)
            cerr << i << " -> " << sum_[i] << " " << lazy_[i] << "\n";
    }

    int query(int from, int to) {
        //cerr << "query " << from << " " << to << "\n";
        return query(1, 0, size_, from, to + 1);
    }

  private:
    int smallestPowerOf2BiggerThanOrEqual(const int &value) {
        int i;
        for (i = 1; i < value; i *= 2);
        return i;
    }

    void add(int node, int begin, int end, int from, int to, int value) {
        if (from <= begin and end <= to) {
            lazy_[node] += value;
            sum_[node] = (sum_[node] + value * (end - begin)) % kModulo;
            return;
        }

        int mid = (begin + end) / 2;
        if (from < mid)
            add(node * 2, begin, mid, from, to, value);
        if (mid < to)
            add(node * 2 + 1, mid, end, from, to, value);
        sum_[node] = (1LL * (end - begin) * lazy_[node] + sum_[node * 2] + sum_[node * 2 + 1]) % kModulo;
    }

    int query(int node, int begin, int end, int from, int to, int lazyAdd = 0) {
        //cerr << " -> " << node << " (" << begin << ", " << end << ")  on  (" << from << ", " << to << ") with lazy " << lazyAdd << "\n";
        if (from <= begin and end <= to) {
            return (1LL * lazyAdd * (end - begin) + sum_[node]) % kModulo;
        }

        int mid = (begin + end) / 2;
        int answer = 0;
        lazyAdd += lazy_[node];
        if (from < mid)
            answer += query(node * 2, begin, mid, from, to, lazyAdd);
        if (mid < to)
            answer += query(node * 2 + 1, mid, end, from, to, lazyAdd);
        return answer % kModulo;
    }

    int size_;
    vector<int> sum_, lazy_;
};

class SuffixAutomaton {
  public:
    SuffixAutomaton(int maxSize = 1):
        lengths_(maxSize + 1) {
        last_ = root_ = new Node(0);
    }

    void extend(char ch) {
        Node* now = new Node(last_->length + 1);

        Node *k;
        for (k = last_; k != nullptr and k->edges.count(ch) == 0; k = k -> link)
            k -> edges[ch] = now;
        last_ = now;

        if (k == nullptr) {
            now -> link = root_;
            lengths_.add(1, now -> length, 1);
            return;
        }

        Node *p = k->edges[ch];
        if (k->length + 1 == p->length) {
            now -> link = p;
            lengths_.add(p -> length + 1, now -> length, 1);
            return;
        }

        Node *clone = new Node(k->length + 1, p->link, p->edges);

        for (; k != nullptr and k->edges[ch] == p; k = k->link)
            k->edges[ch] = clone;

        now -> link = p -> link = clone;
        lengths_.add(clone->length + 1, now -> length, 1);
    }

    int substrings(int from, int to) {
        return lengths_.query(from, to);
    }

  private:
    class Node {
      public:
        Node(int _length, Node * const _link, const map<char, Node*> &_edges):
            length(_length), link(_link), edges(_edges) {
        }

        Node(int _length, Node * const _link = nullptr):
            length(_length), link(_link) {
        }

        int length;
        Node *link;
        map<char, Node*> edges;
    };

    Node *root_, *last_;
    SegmentTree lengths_;
};

int main() {
    int Q; cin >> Q;

    string S;
    vector<char> Op;
    int deleted = 0;

    for (int i = 0; i < Q; ++i) {
        char type; cin >> type;

        if (type == '+') {
            char ch; cin >> ch;
            S += ch;
        } else
            ++deleted;
        Op.push_back(type);
    }

    SuffixAutomaton A(S.size());

    int length = 0;
    deleted = S.size() - deleted;
    while (deleted--) {
        A.extend(S.back());
        S.pop_back();
        ++length;
    }

    reverse(Op.begin(), Op.end());
    int start = 1;

    int answer = 0;
    for (auto &op: Op) {
        cerr << " =====> (" << start << ", " << length << ") => " << A.substrings(start, length) << "\n";
        answer += A.substrings(start, length);
        answer %= kModulo;
        if (op == '+')
            start++;
        else {
            A.extend(S.back());
            S.pop_back();
            ++length;
        }
    }

    if (answer < 0)
        answer += kModulo;
    cout << answer << "\n";
}
