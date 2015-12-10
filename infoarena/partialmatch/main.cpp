#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

class SuffixAutomaton {
  public:
    struct Node {
        Node(int _length = 0, const map<char, Node *> _sons = map<char, Node *>(), Node *_link = nullptr):
                length(_length),
                sons(_sons),
                link(_link) {
        }

        int length;
        map<char, Node *> sons;
        Node *link;

        int depth = 0;
        vector<Node *> links;
    };

    SuffixAutomaton(const string& S):
            m_root(new Node),
            m_last(m_root),
            m_nodes(S.size() + 1) {
        m_nodes[0].push_back(m_root);
        for (auto &x : S)
            extend(x);
    }

    vector< pair<Node *, int> > longestMatch(const string& S) {
        int match = 0;
        Node *now = m_root;

        vector<pair< Node *, int> > answer;
        answer.reserve(S.size());
        for (auto &x : S) {
            while (now != m_root && !now->sons.count(x)) {
                now = now->link;
                match = now->length;
            }
            if (now->sons.count(x)) {
                ++match;
                now = now->sons[x];
            }

            answer.emplace_back(now, match);
        }
        return answer;
    }

    int lcs(pair<Node *, int> one, int prefix) {
        return min(lcs(one.first, m_nodes[prefix].front()), one.second);
    }

    int lcs(Node *one, Node *two) {
        if (one -> depth < two->depth)
            return lcs(two, one);

        int maxLog = one->links.size();
        for (int i = 0; i < maxLog; ++i)
            if ((1 << i) & (one->depth - two->depth))
                one = one->links[i];

        if (one == two)
            return one->length;

        for (int i = maxLog - 1; one != two && i >= 0; --i)
            if ((1 << i) <= one->depth)
                if (one->links[i] != two->links[i]) {
                    one = one->links[i];
                    two = two->links[i];
                }

        one = one->link;
        return one->length;
    }

    void prepareLcs() {
        m_root->depth = 0;

        int log = 2;
        for (int aux = m_nodes.size(); aux; aux /= 2, ++log);

        m_root->links = vector<Node *>(log, nullptr);
        for (int i = 1; i < int(m_nodes.size()); ++i) {
            for (auto &node : m_nodes[i]) {
                node->depth = node->link->depth + 1;
                node->links = vector<Node*>(log);
                node->links[0] = node->link;
                for (int j = 1; j < log; ++j)
                    if (node->links[j - 1])
                        node->links[j] = node->links[j - 1]->links[j - 1];
                    else
                        node->links[j] = nullptr;
            }
        }
    }

  private:
   void extend(char x) {
        Node *now = new Node(m_last->length + 1);
        m_nodes[now->length].push_back(now);

        Node *past;
        for (past = m_last; past != nullptr && !past->sons.count(x); past = past->link)
            past->sons[x] = now;

        m_last = now;
        if (past == nullptr) {
            now->link = m_root;
            return;
        }

        Node *next = past->sons[x];
        if (past->length + 1 == next->length) {
            now->link = next;
            return;
        }

        Node *clone = new Node(past->length + 1, next->sons, next->link);
        m_nodes[clone->length].push_back(clone);
        for (; past != nullptr && past->sons[x] == next; past = past->link)
            past->sons[x] = clone;
        now->link = next->link = clone;
    }

    Node *m_root, *m_last;
    vector< vector<Node *> > m_nodes;
};

int main() {
    ifstream cin("partialmatch.in");
    ofstream cout("partialmatch.out");

    string A, B;
    int maxMismatches;
    cin >> B >> A >> maxMismatches;
    SuffixAutomaton S(B);

    auto longestMatch = S.longestMatch(A);
    S.prepareLcs();
    vector<int> answer;
    for (int i = B.size() - 1; i < int(A.size()); ++i) {
        int length = B.size();
        int now = i;
        int mismatches = maxMismatches;
        for (; mismatches >= 0 && length > 0;) {
            int lcs = S.lcs(longestMatch[now], length);
            now -= lcs;
            length -= lcs;
            if (now < 0)
                break;
            if (length > 0) {
                --now;
                --length;
                --mismatches;
            }
        }

        if (length == 0 && mismatches >= 0)
            answer.push_back(now + 1);
    }

    cout << answer.size() << "\n";
    for (auto &x : answer)
        cout << x << "\n";
}
