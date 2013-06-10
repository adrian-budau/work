#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>

using namespace std;

int main() {
    ifstream cin("timetravel.in");
    ofstream cout("timetravel.out");

    int T; cin >> T;

    map<int, multiset<int> > M;
    set<int> NOW;

    for (int i = 0; i < T; ++i) {
        int type; cin >> type;
        int time, value;
        switch (type) {
            case 1:
                cin >> value;
                NOW.insert(value);
                break;
            case 2:
                cin >> time >> value;
                M[value].insert(time);
                break;
            case 3:
                cin >> value;
                NOW.erase(value);
                break;
            case 4:
                cin >> time >> value;
                M[value].erase(M[value].find(time));
                break;
            case 5:
                cin >> time >> value;
                bool ok = false;
                for (auto it = NOW.lower_bound(value); it != NOW.end(); ++it)
                    if (not M[*it].size() or *M[*it].begin() > time) {
                        cout << *it << "\n";
                        ok = true;
                        break;
                    }
                if (not ok)
                    cout << "Time paradox\n";
        }
    }
}
