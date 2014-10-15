#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <inputGenerator.hpp>

using namespace std;
using namespace inputGenerator;

int main() {
    int N, M; cin >> N >> M;

    vector< vector< pair<int, int> > > d1(N + M + 5), d2(N + M + 5);
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j) {
            auto now = make_pair(i, j);
            d1[i + j].push_back(now);
            d2[i - j + M].push_back(now);
        }

    auto erased1 = [&](pair<int, int> x) {
        int w = x.first + x.second;
        d1[w].erase(find(d1[w].begin(), d1[w].end(), x));
    };

    auto erased2 = [&](pair<int, int> x) {
        int w = x.first - x.second + M;
        d2[w].erase(find(d2[w].begin(), d2[w].end(), x));
    };

    vector< vector<int> > answer(N, vector<int>(M));
    int best = N * M;
    for (int iterations = 0; iterations < 10; ++iterations) {
        vector< vector<int> > aux(N, vector<int>(M));
        auto cpd1 = d1, cpd2 = d2;
        auto now = make_pair(0, 0);
        erased1(now);
        erased2(now);
        aux[now.first][now.second] = 1;
        map<int, int> count;
        for (int _ = 2; _ <= N * M; ++_) {
            int v1 = now.first + now.second;
            int v2 = now.first - now.second + M;
            for (int i = N + M - 2; i >= 0; --i) {
                vector< pair<int, int> > temp;
                if (v1 - i >= 0)
                    temp.insert(temp.end(), d1[v1 - i].begin(), d1[v1 - i].end());
                if (v1 + i < int(d1.size()))
                    temp.insert(temp.end(), d1[v1 + i].begin(), d1[v1 + i].end());
                if (v2 - i >= 0)
                    temp.insert(temp.end(), d2[v2 - i].begin(), d2[v2 - i].end());
                if (v2 + i < int(d2.size()))
                    temp.insert(temp.end(), d2[v2 + i].begin(), d2[v2 + i].end());
                if (temp.size() == 0)
                    continue;
                auto next = randomElement(temp);
                erased1(next);
                erased2(next);
                aux[next.first][next.second] = _;
                count[abs(next.first - now.first) + abs(next.second - now.second)]++;
                now = next;
                break;
            }
        }
        int bestnow = 0;
        for (auto &p : count)
            bestnow = max(bestnow, p.second);
        if (bestnow < best) {
            best = bestnow;
            answer = aux;
        }

        swap(cpd1, d1);
        swap(cpd2, d2);
    }
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j)
            cout << answer[i][j] << " ";
        cout << "\n";
    }
}
