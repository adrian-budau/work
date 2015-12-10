#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <numeric>

using namespace std;

class SegmentJoiner {
  public:
    SegmentJoiner(const vector<int>& values):
            m_smallest(values), m_largest(values),
            m_begin(values.size()), m_end(values.size()) {
        iota(m_begin.begin(), m_begin.end(), 0);
        iota(m_end.begin(), m_end.end(), 0);
    }

    int join(int where) {
        int a = m_begin[where];
        int b = where;
        int c = b + 1;
        int d = m_end[c];
        m_end[a] = d;
        m_begin[d] = a;
        m_smallest[a] = min(m_smallest[a], m_smallest[c]);
        m_largest[d] = max(m_largest[d], m_largest[b]);
        return m_largest[d] - m_smallest[a] + 1;
    }

  private:
    vector<int> m_smallest, m_largest, m_begin, m_end;
};

int answer(vector<string> S) {
    int N = S.size();
    int M = S[0].size();

    // the columns sorted lexicographically
    vector<int> order(M);
    iota(order.begin(), order.end(), 0);
    int answer = 0;

    // the lcp between consecutive columns in the sorted vector
    vector<int> lcp(M - 1, 0);

    vector< vector<int> > whereLcp(N + 1);
    for (int i = 0; i <= N; ++i)
        whereLcp[i].reserve(M); // some great speedup, we removed all possible allocations

    for (int i = 0; i < N; ++i) {
        vector<int> newLcp; newLcp.reserve(M - 1);
        vector<int> newOrder; newOrder.reserve(M);

        auto updateOrder = [&](char c) {
            int last = -1, currentLcp = 0;
            for (int j = 0; j < M; ++j) {
                if (S[i][order[j]] == c) {
                    newOrder.push_back(order[j]);
                    if (last != -1)
                        newLcp.push_back(currentLcp);
                    currentLcp = lcp[j] + 1;
                    last = j;
                }
                currentLcp = min(currentLcp, lcp[j] + 1);
            }
        };

        updateOrder('0');
        if (newOrder.size() > 0 && int(newOrder.size()) < M)
            newLcp.push_back(0);
        updateOrder('1');

        swap(lcp, newLcp);
        swap(order, newOrder);

        for (int i = 1; i < M; ++i)
            whereLcp[lcp[i - 1]].push_back(i - 1);

        SegmentJoiner J(order);
        for (int j = i + 1; j > 0; --j) {
            for (auto &x : whereLcp[j]) {
                answer = max(answer, J.join(x) * j);
            }
            whereLcp[j].clear();
        }
    }
    return max(answer, N);
}

int main() {
    ifstream cin("poveste.in");
    ofstream cout("poveste.out");

    int T; cin >> T;
    while (T--) {
        int N, M; cin >> N >> M;

        vector<string> S(N);
        for (int i = 0; i < N; ++i)
            cin >> S[i];

        cout << answer(S) << "\n";
    }
}
