#include <iostream>
#include <fstream>
#include <vector>
#include <limits>

using namespace std;

class Trie {
  public:
    Trie():
        left(nullptr),
        right(nullptr),
        value(-1) {
    }

    Trie *left = nullptr, *right = nullptr;
    int value = -1;
};

int main() {
    ifstream cin("input.txt");
    ofstream cout("output.txt");

    int N; cin >> N;

    Trie *root = new Trie();

    int answer = numeric_limits<int>::max();
    pair<int, int> where;

    vector<int> V(N);
    for (int i = 0; i < N; ++i) {
        cin >> V[i];

        Trie *now = root;
        if (i > 0) {
            for (int j = 30; j >= 0; --j) {
                if (now -> left == nullptr) {
                    now = now -> right;
                }
                else if (now -> right == nullptr) {
                    now = now -> left;
                }
                else if ((1 << j) & V[i]) {
                    now = now -> left;
                }
                else {
                    now = now -> right;
                }
            }
            
            int tmp = V[i] ^ V[now -> value];
            if (tmp < answer) {
                answer = tmp;
                where = {now -> value, i};
            }
        }

        now = root;
        for (int j = 30; j >= 0; --j)
            if ((1 << j) & V[i]) {
                if (now -> left == nullptr)
                    now -> left = new Trie();
                now = now -> left;
            } else {
                if (now -> right == nullptr)
                    now -> right = new Trie();
                now = now -> right;
            }

        now -> value = i;
    }

    cout << where.first + 1 << " " << where.second + 1 << "\n";
}
