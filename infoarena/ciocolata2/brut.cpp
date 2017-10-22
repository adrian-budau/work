#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <cstring>

using namespace std;

static const int dx[4] = {-1, 0, 1, 0};
static const int dy[4] = {0, -1, 0, 1};

bool go(const vector< vector<int> > &edges, vector<int> &left, vector<int> &right, vector<bool>& used, int node) {
    if (used[node])
        return false;
    used[node] = true;
    for (auto &x : edges[node])
        if (right[x] == -1) {
            right[x] = node;
            left[node] = x;
            return true;
        }
    for (auto &x : edges[node])
        if (go(edges, left, right, used, right[x])) {
            right[x] = node;
            left[node] = x;
            return true;
        }
    return false;
}

void match(const vector< vector<int> > &edges, vector<int>& left, vector<int> &right) {
    vector<bool> used(edges.size());
    do {
        fill(used.begin(), used.end(), false);
        bool changed = false;
        for (int i = 0; i < int(edges.size()); ++i)
            if (left[i] == -1)
                changed |= go(edges, left, right, used, i);
        if (!changed)
            break;
    } while(true);
}

void erase_edge(vector<int>& x, int y) {
    auto it = find(x.begin(), x.end(), y);
    if (it != x.end())
        x.erase(it);
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
    vector< pair<int, int> > left_by_number, right_by_number;
    int K1 = 0, K2 = 0;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j)
            if ((i + j) % 2) {
                number[i][j] = K2++;
                right_by_number.emplace_back(i, j);
            } else {
                number[i][j] = K1++;
                left_by_number.emplace_back(i, j);
            }

    vector< vector<int> > edges(K1);
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j)
            if ((i + j) % 2 == 0)
                for (int k = 0; k < 4; ++k) {
                    int newi = i + dx[k];
                    int newj = j + dy[k];
                    if (newi < 0 || newj < 0 || newi >= N || newj >= M)
                        continue;
                    edges[number[i][j]].push_back(number[newi][newj]);
                }

    vector<int> left(K1, -1), right(K2, -1);
    match(edges, left, right);
    auto print = [&](int left, int right) {
        cout
            << left_by_number[left].first + 1 << " " 
            << left_by_number[left].second + 1 << " " 
            << right_by_number[right].first + 1 << " "
            << right_by_number[right].second + 1 << "\n";
    };

    cout << K1 << "\n";
    for (int i = 0; i < K1; ++i)
        print(i, left[i]);

    int deleted = 0;

    while (K--) {
        int current; assert(cin >> current);
        for (int i = 0; i < current; ++i) {
            int x, y; assert(cin >> x >> y);
            assert(1 <= x && x <= N);
            assert(1 <= y && y <= M);
            --x; --y;
            assert(in[x][y]);
            in[x][y] = false;
            if ((x + y) % 2 == 0)
                edges[number[x][y]].clear();
            else
                for (int k = 0; k < 4; ++k) {
                    int newx = x + dx[k];
                    int newy = y + dy[k];
                    if (newx < 0 || newy < 0 || newx >= N || newy >= M)
                        continue;
                    erase_edge(edges[number[newx][newy]], number[x][y]);
                }
        }
        deleted += current;

        vector<int> new_left(K1, -1), new_right(K2, -1);
        match(edges, new_left, new_right);
        vector<int> erased;
        if (count(new_left.begin(), new_left.end(), -1) + count(new_right.begin(), new_right.end(), -1) != deleted) {
            cout << "-1\n";
            return 0;
        }

        for (int i = 0; i < K1; ++i)
            if (new_left[i] != left[i] && left[i] != -1)
                erased.push_back(i);

        cout << erased.size() << "\n";
        for (auto &x : erased)
            print(x, left[x]);

        vector<int> added;
        for (int i = 0; i < K1; ++i)
            if (new_left[i] != left[i] && new_left[i] != -1)
                added.push_back(i);
        cout << added.size() << "\n";
        for (auto &x : added)
            print(x, new_left[x]);
        swap(left, new_left);
        swap(right, new_right);
    }
}
