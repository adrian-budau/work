#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <algorithm>
#include <cassert>

using namespace std;

int main() {
    ifstream cin("triconnect.in");
    ofstream cout("triconnect.out");

    int N; cin >> N;

    vector<long long> ways(N + 1, 0);
    ways[1] = 1;
    ways[0] = 1;
    for (int i = 2; i <= N; ++i) {
        for (int j = 2; j <= 3 * i; j += 3)
            for (int k = j + 1; k <= 3 * i; k += 3)
                ways[i] += ways[(j - 1) / 3] * ways[(k - j) / 3] * ways[(3 * i + 1 - k) / 3];
    }

    vector<int> first(3 * N), second(3 * N);
    vector< array<int, 3> > combination(N);
    for (int i = 0; i < 3 * N; ++i)
        cin >> first[i];

    for (int i = 0; i < N; ++i)
        for (int j = 0; j < 3; ++j)
            cin >> combination[i][j];

    for (int i = 0; i < 3 * N; ++i)
        cin >> second[i];

    array< vector< pair<int, int> >, 3> left;
    for (int i = 0; i < 3 * N; ++i)
        left[i % 3].emplace_back(first[i], i);

    for (int i = 0; i < 3; ++i)
        sort(left[i].begin(), left[i].end());

    auto many = [&](int x, int y) {
        int count = 0;
        for (int i = 0; i < 3; ++i)
            for (auto z : left[i]) {
                if (x < y) {
                    if (x < z.second && z.second < y)
                        ++count;
                } else {
                    if (x < z.second || z.second < y)
                        ++count;
                }
            }
        return count / 3;
    };

    vector<bool> wall(3 * N, false);
    auto bad = [&](int x, int y, int z) {
        if (x < y) {
            if (x < z && z < y)
                return true;
        } else {
            if (z > x || z < y)
                return true;
        }

        int many_bad = 0;
        for (int i = x; i != y; i = (i + 1) % (3 * N))
            if (wall[i]) {
                ++many_bad;
                break;
            }

        for (int i = y; i != z; i = (i + 1) % (3 * N))
            if (wall[i]) {
                ++many_bad;
                break;
            }

        for (int i = z; i != x; i = (i + 1) % (3 * N))
            if (wall[i]) {
                ++many_bad;
                break;
            }

        if (many_bad >= 2)
            return true;

        vector<bool> new_wall = wall;
        if (max({many(x, y), many(y, z), many(z, x)}) != int(left[0].size() + left[1].size() + left[2].size()) / 3 - 1) {
            new_wall[x] = true;
            new_wall[y] = true;
            new_wall[z] = true;
        }

        for (int i = 0; i < 3 * N; ++i)
            if (new_wall[i]) {
                int count = 0;
                for (int j = (i + 1) % (3 * N); !new_wall[j]; j = (j + 1) % (3 * N))
                    ++count;
                if (count % 3)
                    return true;
            }
        return false;
    };

    long long current = 0;
    for (int step = 0; step < N; ++step) {
        sort(left.begin(), left.end(), [&](const vector< pair<int, int> > &first, const vector< pair<int, int> > &second) {
            return first.front() < second.front();
        });

        if ((left[0].front().second + 1) % 3 != left[1].front().second % 3)
            swap(left[1], left[2]);

        bool ok = true;
        for (auto x : left[1]) {
            for (auto y : left[2]) {
                if (bad(left[0].front().second, x.second, y.second))
                    continue;
                int z = x.first;
                int t = y.first;
                if (z > t)
                    swap(z, t);
                if (combination[step] == array<int, 3>{{left[0].front().first, z, t}}) {
                    ok = false;

                    if (max({many(left[0].front().second, x.second), many(x.second, y.second), many(y.second, left[0].front().second)}) != (N - step - 1)) {
                        wall[x.second] = true;
                        wall[y.second] = true;
                        wall[left[0].front().second] = true;
                    }

                    left[0].erase(left[0].begin());
                    left[1].erase(find(left[1].begin(), left[1].end(), x));
                    left[2].erase(find(left[2].begin(), left[2].end(), y));

                    break;
                }

                current += ways[many(left[0].front().second, x.second)] * ways[many(x.second, y.second)] * ways[many(y.second, left[0].front().second)];
                assert(current < ways[N]);
            }
            if (!ok)
                break;
        }
    }

    assert(current < ways[N]);
    assert(left[0].size() + left[1].size() + left[2].size() == 0);
    for (int i = 0; i < 3 * N; ++i)
        left[i % 3].emplace_back(second[i], i);

    for (int i = 0; i < 3; ++i)
        sort(left[i].begin(), left[i].end());

    vector< array<int, 3> > answer(N);
    wall = vector<bool>(3 * N, false);

    for (int step = 0; step < N; ++step) {
        sort(left.begin(), left.end(), [&](const vector< pair<int, int> > &first, const vector< pair<int, int> > &second) {
            return first.front() < second.front();
        });

        if ((left[0].front().second + 1) % 3 != left[1].front().second % 3)
            swap(left[1], left[2]);

        bool ok = true;
        for (auto x : left[1]) {
            for (auto y : left[2]) {
                if (bad(left[0].front().second, x.second, y.second))
                    continue;

                int z = x.first;
                int t = y.first;
                if (z > t)
                    swap(z, t);

                long long now = ways[many(left[0].front().second, x.second)] * ways[many(x.second, y.second)] * ways[many(y.second, left[0].front().second)];
                if (now <= current) {
                    current -= now;
                    continue;
                }

                if (max({many(left[0].front().second, x.second), many(x.second, y.second), many(y.second, left[0].front().second)}) != (N - step - 1)) {
                    wall[x.second] = true;
                    wall[y.second] = true;
                    wall[left[0].front().second] = true;
                }

                answer[step] = array<int, 3>{{left[0].front().first, z, t}};
                left[0].erase(left[0].begin());
                left[1].erase(find(left[1].begin(), left[1].end(), x));
                left[2].erase(find(left[2].begin(), left[2].end(), y));
 
                ok = false;
                break;
            }
            if (!ok)
                break;
        }
    }

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < 3; ++j) {
            assert(answer[i][j] != 0);
            cout << answer[i][j] << " ";
        }
        cout << "\n";
    }
}
