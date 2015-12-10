#include <iostream>
#include <fstream>
#include <map>

using namespace std;

class Automaton {
  public:
    Automaton() {
        m_root = m_last = new Node{0};
        m_different = 0;
    }

    void push(char ch) {
        Node *now = new Node{m_last->length + 1};

        Node *k;
        for (k = m_last; k && k->next.count(ch) == 0; k = k->link)
            k->next[ch] = now;
        m_last = now;

        if (!k) {
            now->link = m_root;
            m_different += now->length;
            return;
        }

        Node *p = k->next[ch];
        if (k->length + 1 == p->length) {
            now->link = p;
            m_different += now->length - p->length;
            return;
        }

        Node *clone = new Node{k->length + 1, p->link};
        m_different += now->length - clone->length;
        clone->next = p->next;
        for (; k && k->next[ch] == p; k = k->link)
            k->next[ch] = clone;
        p->link = now->link = clone;
    }

    int64_t different() const {
        return m_different;
    }

  private:
    struct Node {
        Node(int _length, Node *_link = nullptr):
                length(_length),
                link(_link) {
        }

        int length;
        map<char, Node*> next;
        Node *link;
    };

    Node *m_root, *m_last;

    int64_t m_different;
};

int main() {
    ifstream cin("prefix2.in");
    ofstream cout("prefix2.out");

    string S; cin >> S;

    Automaton A;
    for (auto &c : S) {
        A.push(c);
        cout << A.different() << "\n";
    }
}
