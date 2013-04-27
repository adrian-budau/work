#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <cstring>

using namespace std;

class SuffixAutomaton {
  public:
    class element {
      public:
        element(const int &_length = 0, element* const _link = 0, const int &_endings = 0) {
            length = _length;
            link = _link;
            endings = _endings;
            solved = false;
            memset(edges, 0, sizeof(edges));
        }

        int length;
        element* link;
        int endings;
        element* edges[52];
        bool solved;
    };

    SuffixAutomaton(const string &S = "") {
        root = new element();
        lastNode = root;
        for (string::const_iterator c = S.begin(); c != S.end(); ++c) {
            int number = 0;
            if (*c >= 'a' && *c <= 'z')
                number = *c - 'a';
            else
                number = *c - 'A' + 26;

            extend(number);
        }
    }

    void prepare() {
        for (element* i = lastNode; i != 0; i = i -> link)
            i -> endings = 1;

        getAnswer(root);
    }

    int many(string S) {
        element *p = root;
        for (string::iterator c = S.begin(); c != S.end(); ++c) {
            int number = 0;
            if (*c >= 'a' && *c <= 'z')
                number = *c - 'a';
            else
                number = *c - 'A' + 26;

            if (!p -> edges[number])
                return 0;
            p = p -> edges[number];
        }

        return p -> endings;
    }

  private:
    void extend(int number) {
        element *now = new element(lastNode -> length + 1);

        element* k;
        for (k = lastNode; k != 0 && !k -> edges[number]; k = k -> link)
            k -> edges[number] = now;

        lastNode = now;

        if (k == 0) {
            now -> link = root;
            return;
        }

        element* next = k -> edges[number];
        if (k -> length + 1 == next -> length) {
            now -> link = next;
            return;
        }

        // we don't have a possible link, so we clone the node that should follow k
        element *clone = new element(k -> length + 1, next -> link);
        memcpy(clone -> edges, next -> edges, sizeof(int) * 52);

        for (; k != 0 && k -> edges[number] == next; k = k -> link)
            k -> edges[number] = clone;
        next -> link = now -> link = clone;
    }

    int getAnswer(element *nod) {
        if (nod -> solved)
            return nod -> endings;
        for (int i = 0; i < 52; ++i)
            if (nod -> edges[i])
                nod -> endings += getAnswer(nod -> edges[i]);
        nod -> solved = true;
        return nod -> endings;
    }

    element *root, *lastNode;
};

int main() {
    ifstream cin("seti.in");
    ofstream cout("seti.out");

    int N; cin >> N;
    string S; 
    while (N--) {
        string C; cin >> C;
        S += C;
    }
   
    SuffixAutomaton T(S);

    T.prepare();

    int M; cin >> M;
    for (int i = 0; i < M; ++i) {
        string C; cin >> C;
        cout << T.many(C) << "\n";
    }
}


