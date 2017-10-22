#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <set>
#include <limits>
#include <map>
#include <cassert>

using namespace std;

template<class T>
int index(map<T, int>& M, T x) {
    if (M.count(x))
        return M[x];
    int size = M.size();
    M[x] = size;
    return size;
}

int main() {
    ifstream cin("socks.in");
    ofstream cout("socks.out");

    int N, K; assert(cin >> N >> K);
    assert(1 <= N && N <= 1000);
    assert(1 <= K && K <= 1000);
    map<string, int> colours;
    map<int, int> sizes;

    vector< pair<int, int> > socks;
    for (int i = 0; i < N; ++i) {
        string colour; int size;
        assert(cin >> colour >> size);
        assert(colour.size() <= 20);
        assert(1 <= size && size <= 1000 * 1000 * 1000);
        socks.emplace_back(index(colours, colour), index(sizes, size));
    }

    vector<int> picked_colours(colours.size(), 0), picked_sizes(sizes.size(), 0);
    int answer = 0;
    for (int i = 0; i < K; ++i) {
        int best = numeric_limits<int>::max(); int where = -1;
        random_shuffle(socks.begin(), socks.end());
        for (int j = 0; j < int(socks.size()); ++j) {
            int cost = picked_colours[socks[j].first] + picked_sizes[socks[j].second];
            if (cost < best) {
                best = cost;
                where = j;
            }
        }

        answer += best;
        picked_colours[socks[where].first]++;
        picked_sizes[socks[where].second]++;
        swap(socks[where], socks.back());
        socks.pop_back();
        cout << answer << "\n";
    }
}
