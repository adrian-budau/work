#include <iostream>
#include <tuple>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    string nameA, nameB;
    int goalA, goalB;
    map<string, int> score, goals;

    for (int i = 0; i < 6; ++i) {
        cin >> nameA >> nameB >> goalA >> goalB;
        if (goalA > goalB)
            score[nameA] += 3;
        else if (goalB > goalA)
            score[nameB] += 3;
        else {
            score[nameA] += 1;
            score[nameB] += 1;
        }

        goals[nameA] += goalA;
        goals[nameB] += goalB;
    }

    vector< tuple<int, int, string> > teams;
    for (auto &x : goals)
        teams.push_back(make_tuple(-score[x.first], -goals[x.first], x.first));
    sort(teams.begin(), teams.end());
    for (auto &p : teams)
        cout << get<2>(p) << "\n";
}
