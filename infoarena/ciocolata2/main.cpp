#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <cstring>

using namespace std;

static const int dx[4] = {-1, 0, 1, 0};
static const int dy[4] = {0, -1, 0, 1};

bool dfs(vector< vector<int> > &edges, vector<bool> &used, int node, int dest) {
    if (node == dest)
        return true;
    if (used[node])
        return false;
    used[node] = true;
    for (int i = 0; i < int(edges[node].size()); ++i)
        if (dfs(edges, used, edges[node][i], dest)) {
            edges[edges[node][i]].push_back(node);
            swap(edges[node][i], edges[node].back());
            edges[node].pop_back();
            return true;
        }
    return false;
}

class Writer {
  public:
    Writer(ostream& stream):
        m_stream(stream) {
        memset(m_buffer, 0, sizeof(m_buffer));
        m_pos = 0;
    }

    Writer& operator<<(int a) {
        int many = 0;
        do {
            digit_buffer[many++] = a % 10 + '0';
            a /= 10;
        } while (a > 0);
        for (int i = many - 1; i >= 0; --i)
            putchar(digit_buffer[i]);
        return *this;
    }

    Writer& operator<<(const char *s) {
        for (; *s; ++s)
            putchar(*s);
        return *this;
    }

    ~Writer() {
        m_stream.write(m_buffer, m_pos);
    }

  private:
    void putchar(char c) {
        m_buffer[m_pos++] = c;
        if (m_pos == kBufferSize) {
            m_stream.write(m_buffer, m_pos);
            m_pos = 0;
        }
    }

    static const int kBufferSize = 32768;
    ostream& m_stream;
    char m_buffer[kBufferSize];
    char digit_buffer[30];
    int m_pos;
};

int main() {
    ifstream cin("ciocolata2.in");
    ofstream out_stream("ciocolata2.out");
    Writer cout(out_stream);

    int N, M, K; assert(cin >> N >> M >> K);
    assert(1 <= N && N <= 70);
    assert(1 <= M && M <= 70);
    assert(1 <= K && K <= 1700);
    assert(N * M % 2 == 0);
    vector< vector<bool> > in(N, vector<bool>(M, true));
    vector< vector<int> > number(N, vector<int>(M, 0));
    vector< pair<int, int> > by_number;
    int INDEX = 0;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j) {
            number[i][j] = INDEX++;
            by_number.emplace_back(i, j);
        }

    vector< vector<int> > edges(INDEX + 2);
    int SOURCE = INDEX;
    int DEST = INDEX + 1;
    auto print = [&](int left, int right) {
        cout
            << by_number[left].first + 1 << " " 
            << by_number[left].second + 1 << " " 
            << by_number[right].first + 1 << " "
            << by_number[right].second + 1 << "\n";
    };

    vector< vector< pair<int, int> > > sets(K);
    for (int s = 0; s < K; ++s) {
        int current; assert(cin >> current);
        for (int i = 0; i < current; ++i) {
            int x, y; assert(cin >> x >> y);
            assert(1 <= x && x <= N);
            assert(1 <= y && y <= M);
            --x; --y;
            assert(in[x][y] == true);
            sets[s].emplace_back(x, y);
            in[x][y] = false;
        }
    }

    reverse(sets.begin(), sets.end());
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j)
            if ((i + j) % 2 == 0) {
                for (int k = 0; k < 4; ++k) {
                    int newi = i + dx[k];
                    int newj = j + dy[k];
                    if (newi < 0 || newj < 0 || newi >= N || newj >= M)
                        continue;
                    if (in[i][j] && in[newi][newj])
                        edges[number[i][j]].push_back(number[newi][newj]);
                }
                if (in[i][j])
                    edges[SOURCE].push_back(number[i][j]);
            } else {
                if (in[i][j])
                    edges[number[i][j]].push_back(DEST);
            }

    vector<bool> used(INDEX + 2, false);
    while (true) {
        fill(used.begin(), used.end(), false);
        if (!dfs(edges, used, SOURCE, DEST))
            break;
    }

    auto current = [&]() {
        vector<int> match(N * M, -1);
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < M; ++j)
                if ((i + j) % 2 == 1) {
                    int x = number[i][j];
                    if (edges[x].size() && edges[x][0] != DEST)
                        match[x] = edges[x][0];
                }
        return match;
    };

    vector< vector<int> > matches;
    matches.push_back(current());

    for (auto &s : sets) {
        for (auto &p : s) {
            int x = p.first;
            int y = p.second;
            in[x][y] = true;
            for (int k = 0; k < 4; ++k) {
                int newx = x + dx[k];
                int newy = y + dy[k];
                if (newx < 0 || newy < 0 || newx >= N || newy >= M)
                    continue;
                if (!in[x][y] || !in[newx][newy])
                    continue;
                if ((x + y) % 2 == 0) {
                    edges[number[x][y]].emplace_back(number[newx][newy]);
                } else {
                    edges[number[newx][newy]].emplace_back(number[x][y]);
                }
            }
            if ((x + y) % 2 == 0)
                edges[SOURCE].emplace_back(number[x][y]);
            else
                edges[number[x][y]].emplace_back(DEST);
        }
        while (true) {
            fill(used.begin(), used.end(), false);
            if (!dfs(edges, used, SOURCE, DEST))
                break;
        }
        matches.push_back(current());
    }

    reverse(matches.begin(), matches.end());

    cout << INDEX / 2 << "\n";
    for (int i = 0; i < INDEX; ++i)
        if (matches[0][i] != -1)
            print(i, matches[0][i]);

    int deleted = 0;
    for (int s = 1; s <= K; ++s) {
        deleted += sets[K - s].size();
        int current_matches = 0;
        for (int i = 0; i < INDEX; ++i)
            if (matches[s][i] != -1)
                ++current_matches;
        if (current_matches * 2 + deleted != INDEX) {
            cout << "-1\n";
            return 0;
        }

        vector<int> erased, added;
        for (int i = 0; i < INDEX; ++i)
            if (matches[s][i] != matches[s - 1][i]) {
                if (matches[s][i] != -1)
                    added.push_back(i);
                if (matches[s - 1][i] != -1)
                    erased.push_back(i);
            }

        cout << erased.size() << "\n";
        for (auto &x : erased)
            print(x, matches[s - 1][x]);
        cout << added.size() << "\n";
        for (auto &x : added)
            print(x, matches[s][x]);
    }
}
