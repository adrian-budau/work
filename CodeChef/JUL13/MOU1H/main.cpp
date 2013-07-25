#include <iostream>
#include <vector>
#include <map>

using namespace std;

const int kModulo = 1000000009;

class SuffixAutomaton {
  public:
    SuffixAutomaton(const vector<int> &V):
            root_(new Node),
            last_node_(root_) {
        nodes_.resize(V.size() + 1);
        root_ -> endings = 0;
        nodes_[0].push_back(root_);
        for (vector<int>::const_iterator it = V.begin(); it != V.end(); ++it)
            expand(*it);
    }

    int differentSubstrings() {
        for (int i = nodes_.size() - 1; i >= 0; --i)
            for (vector<Node*>::iterator it = nodes_[i].begin(); it != nodes_[i].end(); ++it) {
                Node *p = *it;
                for (map<int, Node*>::iterator it = p -> next.begin(); it != p -> next.end(); ++it)
                    p -> endings = (p -> endings + it -> second -> endings) % kModulo;
            }
        return root_ -> endings;
    }

  private:
    class Node {
      public:
        Node(int _length = 0, Node * const _link = 0, const map<int, Node*>& _next = kEmpty):
                length(_length),
                link(_link),
                next(_next),
                endings(1) {
        }

        int value, length;
        Node *link;
        map<int, Node*> next;
        int endings;
    };

    const static map<int, Node*> kEmpty;
    void expand(int value) {
        Node *now = new Node(last_node_ -> length + 1);
        nodes_[now -> length].push_back(now);

        Node *k;
        for (k = last_node_; k and  not k -> next.count(value); k = k -> link)
            k -> next[value] = now;

        last_node_ = now;
        if (k == 0) {
            now -> link = root_;
            return;
        }

        Node *p = k -> next[value];
        if (k -> length + 1 == p -> length) {
            now -> link = p;
            return;
        }

        Node *clone = new Node(k -> length + 1, p -> link, p -> next);
        nodes_[clone -> length].push_back(clone);
        now -> link = p -> link = clone;
        for (; k and k -> next[value] == p; k = k -> link)
            k -> next[value] = clone;
    }

    Node *root_, *last_node_;
    vector< vector<Node*> > nodes_;
};
const map<int, SuffixAutomaton::Node*> SuffixAutomaton::kEmpty;

int main() {
    int T; cin >> T;

    for (int i = 0; i < T; ++i) {
        int N; cin >> N;
        vector<int> V(N);
        for (int i = 0; i < N; ++i)
            cin >> V[i];

        for (int i = 1; i < N; ++i)
            V[i - 1] -= V[i];
        V.resize(--N);

        SuffixAutomaton A(V);

        cout << A.differentSubstrings() << "\n";
    }
}
