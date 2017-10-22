#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <vector>

using namespace std;

int main() {
    ifstream cin("badmilk.in");
    ofstream cout("badmilk.out");

    int N, M, D, S; cin >> N >> M >> D >> S;

    map<int, map<int, int> > drinks;
    map<int, set< pair<int, int> > > when;
    map<int, set<int> > people;
    for (int i = 0; i < D; ++i) {
        int p, m, t; cin >> p >> m >> t;
        if (drinks[p].count(m) && drinks[p][m] <= t)
            continue;

        if (drinks[p].count(m))
            when[p].erase(make_pair(drinks[p][m], m));
        drinks[p][m] = t;
        when[p].emplace(t, m);
        people[m].emplace(p);
    }

    map<int, int> pos_sick;
    for (int i = 0; i < S; ++i) {
        int p, t; cin >> p >> t;
        for (auto &e : when[p])
            if (e.first < t)
                pos_sick[e.second]++;
    }

    int answer = 0;
    for (int i = 1; i <= M; ++i)
        if (pos_sick[i] == S)
            answer = max(answer, int(people[i].size()));

    cout << answer << "\n";
}
