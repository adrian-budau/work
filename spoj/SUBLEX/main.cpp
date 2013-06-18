#include <iostream>
#include <vector>
#include <cassert>
#include <cstring>

using namespace std;

unsigned int kInfinite = (1 << 31);

char s[100000];

bool doit = true;
class SuffixAutomaton {
  public:
    SuffixAutomaton(const string& data) {
        root_ = last_node_ = new Node(0);

        Nodes.resize(data.size() + 1);
        if (doit)
            Nodes[0].push_back(root_);

        for (string::const_iterator it = data.begin(); it != data.end(); ++it)
            extend(*it - 'a');
    }

    void prepare() {
        for (int i = int(Nodes.size()) - 1; i >= 0; --i)
            for (vector<Node*>::iterator it = Nodes[i].begin(); it != Nodes[i].end(); ++it) {
                Node *p = *it;
                for (int j = 0; j < 26; ++j)
                    if (p->next[j]) {
                        p->total += p->next[j]->total;
                        if (p -> total >= kInfinite) {
                            p -> total = kInfinite;
                            break;
                        }
                    }
                if (p -> total < kInfinite)
                    p->total += 1;
            }
    }

    void kthSubstring(unsigned which) {
        which++; // so we pass the null substring
        assert(which <= root_->total);
        Node *k = root_;

        int index = 0;
        while (which > 1) {
            --which;
            for (int i = 0; i < 26; ++i)
                if (k->next[i]) {
                    if (k->next[i]->total < which)
                        which -= k->next[i]->total;
                    else {
                        k = k->next[i];
                        s[index++] = i + 'a';
                        break;
                    }
                }
        }

        s[index++] = 0;
    }

  private:
    class Node {
      public:
        Node(const int &_length, Node * const _link = 0, Node** const _next = 0):
                length(_length),
                link(_link),
                total(0) {
            if (not _next)
                memset(next, 0, sizeof(next));
            else
                memcpy(next, _next, sizeof(next));
        }

        int length;
        Node *link;
        Node *next[26];

        unsigned total;
    };

    void extend(int edge) {
        Node *now = new Node(last_node_->length + 1);
        if (doit)
            Nodes[now->length].push_back(now);

        Node *k;
        for (k = last_node_; k and not k->next[edge]; k = k->link)
            k->next[edge] = now;

        last_node_ = now;

        if (k == 0) {
            now -> link = root_;
            return;
        }

        Node *p = k -> next[edge];
        if (k -> length + 1 == p -> length) {
            now -> link = p;
            return;
        }

        Node *clone = new Node(k -> length + 1, p -> link, p -> next);
        if (doit)
            Nodes[clone->length].push_back(clone);
        p -> link = now -> link = clone;
        for (; k and k->next[edge] == p; k = k->link)
            k->next[edge] = clone;
    }

    Node *root_, *last_node_;

    vector< vector<Node*> > Nodes;
};


int main() {
    scanf("%s", s);
    string S(s);
    SuffixAutomaton A(S);

    A.prepare();

    int Q; cin >> Q;
    for (int i = 0; i < Q; ++i) {
        int K; scanf("%d", &K);
        A.kthSubstring(K);
        printf("%s\n", s);
    }
}
