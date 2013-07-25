#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

const int kInfinite = 0x3f3f3f3f;

template<class T>
class by_array {
  public:
    by_array(const vector<T> &array):
            array_(array) {
    }

    bool operator()(const int &x, const int &y) const {
        return array_[x] < array_[y];
    }

  private:
    const vector<T>& array_;
};

int main() {
    int T; cin >> T;

    while (T--) {
        int N, M, W; cin >> N >> M >> W;

        vector< pair<int, int> > P(N);
        for (int i = 0; i < N; ++i)
            cin >> P[i].first >> P[i].second;

        vector<int> R(M);
        vector<int> C(M);
        for (int i = 0; i < M; ++i) {
            cin >> R[i] >> C[i];
        }

        for (int i = 0; i < M; ++i)
            for (int j = i + 1; j < M; ++j)
                if (R[i] > R[j]) {
                    swap(R[i], R[j]);
                    swap(C[i], C[j]);
                }
        vector< vector<long long> > distance(N, vector<long long>(N, 0));
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                distance[i][j] = 1LL * (P[i].first - P[j].first) * (P[i].first - P[j].first) + 1LL * (P[i].second - P[j].second) * (P[i].second - P[j].second);


        vector< vector<int> > points(N, vector<int>(N));
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j)
                points[i][j] = j;
            sort(points[i].begin(), points[i].end(), by_array<long long>(distance[i]));
        }

        vector< vector<int> > cost(N, vector<int>(M, kInfinite));
        vector<int> can(N, M);
        vector<int> best_of(N, kInfinite);
        vector<int> where_of(N, M);
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < M; ++j)
                if (P[i].second <= R[j]) {
                    cost[i][j] = 0;
                    best_of[i] = 0;
                    where_of[i] = min(where_of[i], j);
                }

        while (true) {
            int best = 0x3f3f3f3f;
            int pile = -1;
            int disk = -1;

            for (int i = 0; i < N; ++i)
                if (best_of[i] < best) {
                    best = best_of[i];
                    pile = i;
                    disk = where_of[i];
                }

            if (pile == -1)
                break;

            if (disk + 1 < can[pile]) {
                disk = can[pile] - 1;
                cost[pile][disk] = best;
            }

            int k = 0;
            for (int j = 0; j < N; ++j) {
                int now = points[pile][j];
                if (now == pile)
                    continue;
                while (k < M and distance[pile][now] > 1LL * (R[k] + R[disk]) * (R[k] + R[disk]))
                    ++k;
                if (k == M)
                    break;

                if (cost[pile][disk] + C[disk] < cost[now][k]) {
                    cost[now][k] = cost[pile][disk] + C[disk];
                    if (cost[now][k] < best_of[now]) {
                        best_of[now] = cost[now][k];
                        where_of[now] = k;
                    } else if (cost[now][k] == best_of[now] and k < where_of[now]) {
                        where_of[now] = k;
                    }
                }
            }

            can[pile] = disk;

            best_of[pile] = kInfinite;
            where_of[pile] = kInfinite;
            for (int j = 0; j < can[pile]; ++j)
                if (cost[pile][j] < best_of[pile]) {
                    best_of[pile] = cost[pile][j];
                    where_of[pile] = j;
                } else if(cost[pile][j] == best_of[pile] and j < where_of[pile]) {
                    where_of[pile] = j;
                }
        }

        int answer = kInfinite;
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < M; ++j)
                if (W - P[i].second <= R[j])
                    answer = min(answer, cost[i][j] + C[j]);

        if (answer == kInfinite)
            cout << "impossible\n";
        else
            cout << answer << "\n";
    }
}
