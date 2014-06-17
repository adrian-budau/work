#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int kMaxM = 1005;
const int kModulo = 1000 * 1000 * 1000 + 7;

int comb[kMaxM][kMaxM];

class Trie {
  public:
    Trie(int shards):
            m_root(new Node()),
            m_shards(shards) {
    }

    ~Trie() {
        delete m_root;
    }

    void add(string S) {
        Node *current = m_root;
        for (auto ch : S) {
            current->count++;
            int index = ch - 'A';
            if (current->sons[index] == 0)
                current->sons[index] = new Node();
            current = current->sons[index];
        }

        current->count++;
        current->word = true;
    }

    pair<int, int> solve() {
        int most, many;
        tie(most, many) = solve(m_root);
        if (m_root->count < m_shards)
            many = (1LL * many * comb[m_shards][m_root->count]) % kModulo;
        return {most, many};
    }

  private:
    struct Node {
        Node() {
            memset(sons, 0, sizeof(sons));
            count = 0;
            word = false;
        }

        ~Node() {
            for (int i = 0; i < 26; ++i)
                if (sons[i] != 0)
                    delete sons[i];
        }

        Node *sons[26];
        int count;
        bool word;
    };

    pair<int, int> solve(Node* now) {
        int most = min(now->count, m_shards);
        vector< pair<int, int> > sons;
        for (int i = 0; i < 26; ++i)
            if (now->sons[i]) {
                int nowmost, nowmany;
                tie(nowmost, nowmany) = solve(now->sons[i]);
                most += nowmost;
                sons.emplace_back(min(now->sons[i]->count, m_shards), nowmany);
            }

        if (sons.empty())
            return {most, 1};

        if (now->word)
            sons.emplace_back(1, 1);

        sort(sons.begin(), sons.end());
        if (sons.back().first == m_shards) { // we're lucky
            int answer = 1;
            for (auto p : sons) {
                answer = (1LL * answer * comb[m_shards][p.first]) % kModulo;
                answer = (1LL * answer * p.second) % kModulo;
            }

            return {most, answer};
        }

        if (now->count <= m_shards) { // we're also lucky
            int left = now->count;
            int answer = 1;
            for (auto p : sons) {
                answer = (1LL * answer * comb[left][p.first]) % kModulo;
                answer = (1LL * answer * p.second) % kModulo;
                left -= p.first;
            }
            return {most, answer};
        }

        // damn, this is bad Houston
        // dp[i][j] = with the terms wiht index <= i have filled at least j spots
        vector< vector<int> > dp(sons.size(), vector<int>(m_shards + 1, 0));
        dp[0][sons[0].first] = (1LL * comb[m_shards][sons[0].first] * sons[0].second) % kModulo;
        for (int i = 1; i < int(sons.size()); ++i)
            for (int j = sons[i].first; j <= m_shards; ++j)
                for (int k = 1; k <= j; ++k) {
                    if (j - k > sons[i].first)
                        continue;
                    int aux = (1LL * dp[i - 1][k] * sons[i].second) % kModulo; // the ways we arrange them
                    aux = (1LL * aux * comb[m_shards - k][j - k]) % kModulo; // the ways we pick the new places
                    aux = (1LL * aux * comb[k][sons[i].first - (j - k)]) % kModulo; // the ways we pick the old places
                    dp[i][j] = (dp[i][j] + aux) % kModulo;
                }
        return {most, dp[sons.size() - 1][m_shards]};
    }

    Node *m_root;
    int m_shards;
};

int main() {
    comb[0][0] = 1;
    for (int i = 0; i + 1 < kMaxM; ++i)
        for (int j = 0; j <= i; ++j) {
            comb[i + 1][j] = (comb[i + 1][j] + comb[i][j]) % kModulo;
            comb[i + 1][j + 1] = (comb[i + 1][j + 1] + comb[i][j]) % kModulo;
        }

    int T; cin >> T;

    for (int test = 1; test <= T; ++test) {

        int M, N; cin >> M >> N;
        Trie S(N);
        for (int i = 0; i < M; ++i) {
            string s; cin >> s;
            S.add(s);
        }

        int most, many;
        tie(most, many) = S.solve();
        cout << "Case #" << test << ": " << most << " " << many << "\n";
    }
}
