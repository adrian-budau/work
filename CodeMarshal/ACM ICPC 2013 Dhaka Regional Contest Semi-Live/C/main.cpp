#include <iostream>
#include <algorithm>
#include <limits>
#include <unordered_set>

using namespace std;

const int kInfinite = numeric_limits<int>::max() / 2;

void sort(int &A, int &B) {
    if (A > B)
        swap(A, B);
}

class hasher {
  public:
    hasher() {
    }

    size_t operator()(pair<int, int> a) const {
        return hasher_(a.first) ^ hasher_(a.second);
    }

  private:
    std::hash<int> hasher_;
};

int main() {
    int T; cin >> T;

    for (int test = 1; test <= T; ++test) {
        cout << "Case " << test << ": ";

        int N, M, K; cin >> N >> M >> K;

        vector< vector<int> > dist(N, vector<int>(N, kInfinite));

        for (int i = 0; i < M; ++i) {
            int x, y, z; cin >> x >> y >> z;
            --x; --y;
            dist[x][y] = dist[y][x] = z;
        }

        if (K % 2) {
            cout << "Impossible\n";
            continue;
        }

        for (int k = 0; k < N; ++k)
            for (int i = 0; i < N; ++i)
                for (int j = 0; j < N; ++j)
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);

        unordered_set< pair<int, int>, hasher> match, queue;
        for (int i = 0; i < K; i += 2) {
            match.insert({i, i + 1});
            queue.insert({i, i + 1});
        }

        while (!queue.empty()) {
            int x, y;
            tie(x, y) = *queue.begin();
            queue.erase(queue.begin());

            for (auto &other : match) {
                int now = dist[x][y] + dist[other.first][other.second];

                int first = dist[x][other.first] + dist[y][other.second];
                int second = dist[x][other.second] + dist[y][other.first];

                if (now <= min(first, second))
                    continue;
                int newx, newy, newz, newt;
                if (first < second) {
                    newx = x; newy = other.first;
                    newz = y; newt = other.second;
                } else {
                    newx = x; newy = other.second;
                    newz = y; newt = other.first;
                }

                queue.erase(other);
                match.erase({x, y});
                match.erase(other);

                sort(newz, newt);
                sort(newx, newy);
                match.insert({newx, newy});
                match.insert({newz, newt});
                queue.insert({newx, newy});
                queue.insert({newz, newt});
            }
        }

        bool ok = true;
        for (auto &edge : match)
            if (dist[edge.first][edge.second] == kInfinite) 
                ok = false;

        if (!ok) {
            cout << "Impossible\n";
            continue;
        }

        int answer = 0;
        for (auto &edge : match)
            answer += dist[edge.first][edge.second];
        cout << answer << "\n";
    }
}
