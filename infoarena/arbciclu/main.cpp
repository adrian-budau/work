#include <iostream>
#include <cstring>
#include <set>
#include <vector>
#include <queue>
#include <fstream>

using namespace std;

class Scanner {
  public:
    Scanner(string file, int buffer_size = 32768):
            buffer_size_(buffer_size) {
        file_ = fopen(file.c_str(), "r");
        buffer_ = new char[buffer_size];
        pointer_ = buffer_ + buffer_size_;
    }

    Scanner& operator>>(int &object) {
        object = 0;
        while (next() < '0' or next() > '9')
            advance();
        while (next() >= '0' and next() <= '9') {
            object = object * 10 + next() - '0';
            advance();
        }
        return *this;
    }

  private:
    char next() {
        if (pointer_ == buffer_ + buffer_size_) {
            pointer_ = buffer_;
            fread(buffer_, 1, buffer_size_, file_);
        }
        return *pointer_;
    }

    void advance() {
        ++pointer_;
    }

    FILE *file_;
    int buffer_size_;
    char *buffer_, *pointer_;
};

int main() {
    Scanner cin("arbciclu.in");
    ofstream cout("arbciclu.out");

    int T; cin >> T;
    while (T--) {
        int N, M; cin >> N >> M;
        vector<set<int>> H(N);

        for (int i = 0; i < M; ++i) {
            int x, y; cin >> x >> y;
            --x; --y;
            H[x].insert(y);
            H[y].insert(x);
        }

        bool stop = false;
        for (int i = 0; i < N; ++i)
            if (H[i].size() < 2) {
                cout << "NO\n";
                stop = true;
                break;
            }
        if (stop)
            continue;
        queue<int> Q;
        vector<int> used(N, 0);

        int contractions = 0;

        for (int i = 0; i < N; ++i)
            if (H[i].size() == 2) {
                Q.push(i);
                used[i] = 1;
            }

        while (!Q.empty()) {
            int node = Q.front();
            Q.pop();
            if (H[node].size() < 2) // we should stop at those last two nodes
                break;
            ++contractions;

            int x = *H[node].begin();
            int y = *next(H[node].begin(), 1);

            H[x].erase(node);
            H[y].erase(node);

            H[x].insert(y);
            H[y].insert(x);
            H[node].clear();

            if (H[x].size() == 2 and not used[x]) {
                used[x] = 1;
                Q.push(x);
            }

            if (H[y].size() == 2 and not used[y]) {
                used[y] = 1;
                Q.push(y);
            }
        }

        if (3 <= N and N <= M and contractions == N - 2)
            cout << "YES\n";
        else
            cout << "NO\n";
    }
}
