#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

class FenwickTree {
  public:
    FenwickTree(int size): m_data(size, 0) {
    }

    int size() const {
        return m_data.size();
    }

    void add(int x, int y) {
        for (; x < size(); x += (x + 1 & -x - 1))
            m_data[x] += y;
    }

    int query(int x) {
        int answer = 0;
        for (; x > 0; x -= (x + 1 & -x - 1))
            answer += m_data[x];
        return answer;
    }

  private:
    vector<int> m_data;
};

int main() {
    ifstream cin("subpermutari.in");
    ofstream cout("subpermutari.out");

    int N; assert(cin >> N);
    assert(1 <= N && N <= 2000);

    vector<int> V(N);
    vector<bool> have(N, false);
    for (int i = 0; i < N; ++i) {
        assert(cin >> V[i]);
        assert(1 <= V[i] && V[i] <= N);
        --V[i];
        have[V[i]] = true;
    }

    assert(count(have.begin(), have.end(), false) == 0);

    FenwickTree F(N);

    vector< vector<int> > same(N, vector<int>(N, -1));
    vector<int> normalized(N), pi(N);
    for (int i = 0; i < N; ++i) {
        int k = i, start = i + 1;

        for (int j = i; j < N; ++j) {
            normalized[j] = F.query(V[j]);
            F.add(V[j], 1);
        }
        for (int j = i; j < N; ++j)
            F.add(V[j], -1);

        pi[i] = i;
        for (int j = i + 1; j < N; ++j) {
            while (k > i && F.query(V[j]) != normalized[k]) {
                k = pi[k - 1];
                while (j - start > k - i)
                    F.add(V[start++], -1);
            }
            F.add(V[j], 1);
            pi[j] = ++k;
            same[start][j] = i;
        }

        while (start < N)
            F.add(V[start++], -1);
    }

    for (int i = 0; i < N; ++i)
        for (int j = i + 1; j < N; ++j)
            if (same[i][j] != -1)
                same[i + 1][j] = same[i][j] + 1;
    vector< vector<int> > count(N, vector<int>(N, 1));
    int64_t answer = 0;
    for (int i = N - 1; i >= 0; --i)
        for (int j = i; j < N; ++j)
            if (same[i][j] == -1) {
                answer += count[i][j] * count[i][j];
            } else {
                count[same[i][j]][same[i][j] + j - i] += count[i][j];
                count[i][j] = 0;
            }

    cout << answer << "\n";
}
