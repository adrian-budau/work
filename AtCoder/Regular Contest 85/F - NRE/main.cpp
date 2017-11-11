#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class SegmentTree {
  public:
    SegmentTree(int size, int value):
        m_size(size),
        m_data(size * 2, value) {}

    void update(int position, int value) {
        for (position += m_size; position; position /= 2)
            m_data[position] = min(m_data[position], value);
    }

    int query(int from, int to) {
        int answer = numeric_limits<int>::max();
        for (from += m_size, to += m_size; from < to; from /= 2, to /= 2) {
            if (from & 1)
                answer = min(answer, m_data[from++]);
            if (to & 1)
                answer = min(answer, m_data[--to]);
        }
        return answer;
    }

  private:
    int m_size;
    vector<int> m_data;
};

int main() {
    ios::sync_with_stdio(false);
    int N; cin >> N;
    vector<int> B(N + 1, 0);
    for (int i = 1; i <= N; ++i)
        cin >> B[i];

    vector<int> partial(N + 1, 0);
    for (int i = 1; i <= N; ++i)
        partial[i] = partial[i - 1] + B[i];

    int Q; cin >> Q;
    vector< vector<int> > rights(N + 1);
    for (int i = 0; i < Q; ++i) {
        int x, y; cin >> x >> y;
        rights[x].emplace_back(y);
    }

    SegmentTree S(N + 1, N);
    vector<int> dp(N + 1, N);
    dp[0] = 0;
    for (int i = 1; i <= N; ++i) {
        dp[i] = min(dp[i], dp[i - 1] + B[i]);
        S.update(i - 1, dp[i - 1] - (i - 1) + partial[i - 1]);
        // dp[i] = min(dp[j] + (i - j) - (partial[i] - partial[j]))
        //       = i - partial[i] + min(dp[j] - j + partial[j])
        for (auto &right : rights[i]) {
            int best = right - partial[right] + S.query(i - 1, right);
            dp[right] = min(dp[right], best);
            S.update(right, best - right + partial[right]);
        }
    }

    cout << dp[N] << "\n";
}
