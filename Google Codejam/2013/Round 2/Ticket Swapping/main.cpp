#include <iostream>
#include <map>
#include <vector>
#include <fstream>

using namespace std;

const long long modulo = 1000002013;

int main() {

    int T; cin >> T;
    for (int test = 1; test <= T; ++test) {
        int N, M; cin >> N >> M;

        map< pair<int, int>, long long > S;
        long long answer = 0;
        for (int i = 0; i < M; ++i) {
            int x, y, p; cin >> x >> y >> p;
            S[make_pair(x, y)] += p;
        }

        do {
            answer %= modulo;
            auto now = S.begin();
            auto next = now;
            for (auto it = S.begin(); it != S.end(); ++it) {
                if (it -> first.first != now->first.first and it -> first.first >= now ->first.first and it -> first.first <= now->first.second and it -> first.second > now->first.second) {
                    next = it;
                    break;
                }
            }

            if (now == next) {
                S.erase(now);
                continue;
            }

            if (now -> second == next -> second) {
                int x = now -> first.first;
                int y = next -> first.first;
                int z = now -> first.second;
                int t = next -> first.second;

                answer += (((1LL * (now -> second % modulo) * (y - x)) % modulo) * (t - z)) % modulo;
                int many = now -> second;
                S.erase(now);
                S.erase(next);
                S[make_pair(x, t)] += many;
                S[make_pair(y, z)] += many;
                continue;
            }

            if (now -> second < next -> second) {
                int x = now -> first.first;
                int y = next -> first.first;
                int z = now -> first.second;
                int t = next -> first.second;

                answer += (((1LL * (now -> second % modulo) * (y - x)) % modulo) * (t - z)) % modulo;
                int many = now -> second;
                S.erase(now);
                S[make_pair(x, t)] += many;
                S[make_pair(y, z)] += many;
                S[make_pair(y, t)] -= many;
                continue;
            } 
            int x = now -> first.first;
            int y = next -> first.first;
            int z = now -> first.second;
            int t = next -> first.second;

            answer += (((1LL * (next -> second % modulo) * (y - x)) % modulo) * (t - z)) % modulo;
            int many = next -> second;
            S.erase(next);
            S[make_pair(x, t)] += many;
            S[make_pair(y, z)] += many;
            S[make_pair(x, z)] -= many;
        } while (S.size());

        cout << "Case #" << test << ": " << answer % modulo << "\n";
    }
}
