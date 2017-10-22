#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>

using namespace std;

vector< pair<int, int> > cleanUp(vector<pair<int, int> > segments) {
    sort(segments.begin(), segments.end());
    vector<pair<int, int> > new_segments;
    for (auto &S : segments) {
        if (new_segments.empty() || new_segments.back().second + 1 < S.first)
            new_segments.push_back(S);
        else
            new_segments.back().second = max(new_segments.back().second, S.second);
    }
    return new_segments;
}

vector< pair<int, int> > filter(vector< pair<int, int> > segments, int K) {
    segments.erase(remove_if(segments.begin(), segments.end(), [&](pair<int, int> A) {
        return A.second - A.first + 1 < K;
    }), segments.end());
    return segments;
}

vector< pair<int, int> > join(vector<pair<int, int> > A, vector<pair<int, int>> B) {
    A.insert(A.end(), B.begin(), B.end());
    return cleanUp(A);
}

vector< pair<int, int> > intersect(vector<pair<int, int> > A, vector<pair<int, int>> B) {
    int N = A.size();
    int M = B.size();
    int x = 0;
    int y = 0;
    vector< pair<int, int> > result;
    while (x < N && y < M) {
        if (A[x].second < B[y].first) {
            ++x;
            continue;
        }
        if (B[y].second < A[x].first) {
            ++y;
            continue;
        }
        result.emplace_back(max(A[x].first, B[y].first), min(A[x].second, B[y].second));
        if (A[x].second < B[y].second)
            ++x;
        else
            ++y;
    }
    return result;
}

int main() {
    ios::sync_with_stdio(false);

    int T; cin >> T;
    while (T--) {
        int N, M, K; cin >> N >> M >> K;

        int P; cin >> P;
        map<int, vector<pair<int, int>> > fires;
        for (int i = 0; i < P; ++i) {
            int b, e, y; cin >> b >> e >> y;
            fires[y].emplace_back(b, e);
        }

        for (auto &line : fires) {
            // cleanup the line
            line.second = cleanUp(line.second);
        }

        set<int> interesting;
        for (auto &line : fires)
            for (int i = -2; i <= 2; ++i)
                interesting.emplace(line.first + i);

        // intersect up
        for (auto &y : interesting) {
            auto &line = fires[y];
            line = intersect(fires[y + 1], line);
        }

        // intersect down
        for (auto it = interesting.rbegin(); it != interesting.rend(); ++it) {
            auto& line = fires[*it];
            line = intersect(fires[*it - 1], line);
        }

        int64_t answer = 0;
        for (auto &y : interesting) {
            auto up = filter(fires[y - 1], K);
            auto now = filter(fires[y], K);
            auto down = filter(fires[y + 1], K);
            auto total = join(up, now);
            total = join(total, down);
            for (auto &S : total)
                answer += S.second - S.first + 1;
        }

        cout << answer << "\n";
    }
}
