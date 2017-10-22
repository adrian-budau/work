#include <iostream>
#include <vector>
#include <set>
#include <stack>

using namespace std;

static const int kInfinite = numeric_limits<int>::max();

int main() {
    int T; cin >> T;
    for (int test = 1; test <= T; ++test) {
        cout << "Case #" << test << ":\n";
        int N, M; cin >> N >> M;

        vector< vector< pair<int, int> > > edges(N);
        for (int i = 0; i < M; ++i) {
            int x, y, h; cin >> x >> y >> h;
            --x; --y;
            edges[x].emplace_back(y, h);
            edges[y].emplace_back(x, h);
        }

        set< pair<int, int> > S;
        vector<int> H(N);
        for (int i = 1; i < N; ++i) {
            S.emplace(kInfinite, i);
            H[i] = kInfinite;
        }

        S.emplace(0, 0);
        H[0] = 0;

        vector<int64_t> answer(N, -1);

        int64_t time = 0;
        vector< pair<int, int> > st;
        while (!S.empty()) {
            int node = S.begin() -> second;
            int height = S.begin() -> first;

            while (st.size() && st.back().first < height) {
                if (st.size() == 1 || st[st.size() - 2].first > height) {
                    time += 1LL * st.back().second * (height - st.back().first);
                    st.back().first = height;
                } else {
                    time += 1LL * st.back().second * (st[st.size() - 2].first - st.back().first);
                    st[st.size() - 2].second += st.back().second;
                    st.pop_back();
                }
            }

            if (st.empty() || st.back().first != 0)
                st.push_back(make_pair(0, 1));
            S.erase(S.begin());
            answer[node] = time;

            for (auto &x : edges[node])
                if (answer[x.first] == -1)
                    if (x.second < H[x.first]) {
                        S.erase(make_pair(H[x.first], x.first));
                        H[x.first] = x.second;
                        S.insert(make_pair(H[x.first], x.first));
                    }
        }
        for (int i = 0; i < N; ++i)
            cout << answer[i] << " ";
        cout << "\n";
    }
}

