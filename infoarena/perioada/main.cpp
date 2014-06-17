#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>

using namespace std;

class SuffixArray {
  public:
    SuffixArray(const string& S) {
        int log;
        for (log = 1; (1 << log) < int(S.size()); ++log);

        prefix_ = vector< vector<int> > (log, vector<int>(S.size()));

        for (size_t i = 0; i < S.size(); ++i)
            prefix_[0][i] = S[i] - 'a';

        vector<Triple> V(S.size());
        for (int i = 1; i < log; ++i) {
            for (int j = 0; j < int(S.size()); ++j)
                if (j + (1 << (i - 1)) < int(S.size()))
                    V[j] = {prefix_[i - 1][j], prefix_[i - 1][j + (1 << (i - 1))], j};
                else
                    V[j] = {prefix_[i - 1][j], -1, j};
            sort(V.begin(), V.end());

            for (int j = 0; j < int(S.size()); ++j)
                if (j == 0 or V[j - 1] < V[j])
                    prefix_[i][V[j].index] = j;
                else
                    prefix_[i][V[j].index] = prefix_[i][V[j - 1].index];
        }
    }

    int lcp(int x, int y) {
        int answer = 0;
        for (int i = prefix_.size() - 1; i >= 0; --i)
            if (prefix_[i][x] == prefix_[i][y]) {
                answer += (1 << i);
                x += (1 << i);
                y += (1 << i);
            }
        return answer;
    }

  private:
    struct Triple {
        int x, y, index;

        bool operator<(const Triple &that) const {
            if (x != that.x)
                return x < that.x;
            return y < that.y;
        }
    };

    vector< vector<int> > prefix_;
};

int main() {
    ifstream cin("perioada.in");
    ofstream cout("perioada.out");

    int N; cin >> N;
    string S; cin >> S;

    SuffixArray A(S);

    int M; cin >> M;
    while (M--) {
        int x, y; cin >> x >> y;

        int L = y - x + 1;
        int answer = 0;
        --x; --y;
        for (int i = 1; i * i <= L; ++i)
            if (L % i == 0) {
                if (A.lcp(x, x + i) >= L - i)
                    ++answer;

                if (i != 1 and L / i != i)
                    if (A.lcp(x + L / i, x) >= L - L / i)
                        ++answer;
            }

        cout << answer << "\n";
    }
}
