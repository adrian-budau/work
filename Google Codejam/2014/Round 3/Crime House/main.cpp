#include <iostream>
#include <fstream>
#include <algorithm>
#include <set>
#include <vector>
#include <map>

using namespace std;

int main() {
    int T; cin >> T;
    for (int test = 1; test <= T; ++test) {
        int N; cin >> N;

        set<int> anonymousEnters, anonymousExits;

        vector<string> type(N);
        vector<int> who(N);
        for (int i = 0; i < N; ++i) {
            cin >> type[i] >> who[i];
            if (who[i] == 0) {
                if (type[i] == "E")
                    anonymousEnters.emplace(i);
                else
                    anonymousExits.emplace(i);
            }
        }

        map<int, pair<string, int> > first, last;
        bool crimeTime = false;
        for (int i = N - 1; i >= 0; --i)
            if (who[i] != 0) {
                if (type[i] == "L" && last[who[i]].first == "L") {
                    auto it = anonymousEnters.lower_bound(last[who[i]].second);
                    if (it != anonymousEnters.begin()) {
                        --it;
                        if (*it > i)
                            anonymousEnters.erase(it);
                        else
                            crimeTime = true;
                    } else
                        crimeTime = true;
                }
                last[who[i]] = make_pair(type[i], i);
            }

        vector< pair<int, int> > enterExitPairs;
        last.clear();
        for (int i = 0; i < N; ++i)
            if (who[i] != 0) {
                if (first[who[i]].first == "")
                    first[who[i]] = make_pair(type[i], i);
                if (type[i] == "E" && last[who[i]].first == "E") {
                    auto it = anonymousExits.lower_bound(last[who[i]].second);
                    if (it != anonymousExits.end() && *it < i)
                        anonymousExits.erase(it);
                    else
                        crimeTime = true;
                }
                if (type[i] == "L" && last[who[i]].first == "E")
                    enterExitPairs.emplace_back(last[who[i]].second, i);
                last[who[i]] = make_pair(type[i], i);
            }

        if (crimeTime) {
            cout << "Case #" << test << ": CRIME TIME\n";
            continue;
        }

        for (auto someone : first) {
            if (someone.second.first == "L") { // we can remove an enter from someone anonymous
                auto it = anonymousEnters.lower_bound(someone.second.second);
                if (it != anonymousEnters.begin()) {
                    --it;
                    anonymousEnters.erase(it);
                }
            }
        }

        int answer = 0;
        for (auto someone : last)
            if (someone.second.first == "E") { // we can match this with an exit, a rightmost one best
                auto it = anonymousExits.lower_bound(someone.second.second);
                if (it != anonymousExits.end())
                    anonymousExits.erase(it);
                else
                    ++answer;
            }

        sort(enterExitPairs.begin(), enterExitPairs.end());
        for (auto p : enterExitPairs) {
            auto it = anonymousExits.lower_bound(p.first);
            auto jt = anonymousEnters.lower_bound(p.second);
            if (it != anonymousExits.end() && jt != anonymousEnters.begin()) {
                --jt;
                if (*it < *jt) {
                    anonymousEnters.erase(jt);
                    anonymousExits.erase(it);
                }
            }
        }

        /*
        for (auto exit : anonymousExits) {
            if (anonymousEnters.size() && *anonymousEnters.begin() < exit) {
                anonymousEnters.erase(anonymousEnters.begin());
            }
        }*/
        for (auto enter : anonymousEnters) {
            auto it = anonymousExits.lower_bound(enter);
            if (it != anonymousExits.end())
                anonymousExits.erase(it);
            else
                ++answer;
        }
        cout << "Case #" << test << ": " << answer << "\n";
    }
}
