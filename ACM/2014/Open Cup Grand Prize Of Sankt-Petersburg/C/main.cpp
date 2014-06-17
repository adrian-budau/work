#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <numeric>
#include <set>

using namespace std;

int main() {
    ifstream cin("equal-power.in");
    ofstream cout("equal-power.out");

    int N; cin >> N;
    vector< pair<int, int> > V(N);

    for (int i = 0; i < N; ++i) {
        cin >> V[i].first;
        V[i].second = i + 1;
    }

    sort(V.begin(), V.end());

    auto maximum = V.back();
    int total = 0;
    for (auto x : V)
        total += x.first;
    V.pop_back();

    vector< pair<int, int> > from(total + 1, {-1, -1});
    from[0] = {0, 0};
    vector< pair<int, int> > first, second;
    int power = 0;
    for (auto x : V) {
        auto update = [&]() {
            for (int i = total; i >= x.first; --i)
                if (from[i - x.first] != make_pair(-1, -1) && from[i] == make_pair(-1, -1))
                    from[i] = x;
        };

        // maximum ....   vs  x ......
        // 2 * maximum + Y = 2 * x + Z
        // Y + Z = (total - maximum - x)
        // Y = total - maximum - x - Z
        // 2 * maximum + total - maximum - x - Z = 2 * x + Z
        // maximum + total = 3 * x + 2 * Z
        // Z = (maximum + totaal - 3x) / 2
        int need = maximum.first + total - 3 * x.first;
        if (need % 2) {
            update();
            continue;
        }
        need /= 2;
        if (from[need] == make_pair(-1, -1)) {
            update();
            continue;
        }
        second.push_back(x);

        for (int i = need; i != 0; i -= from[i].first)
            second.push_back(from[i]);

        power = need + 2 * x.first;
        break;
    }

    if (power == 0) {
        cout << "-1\n";
        return 0;
    }

    sort(second.begin(), second.end());
    V.push_back(maximum);

    multiset< pair<int, int> > S(V.begin(), V.end());
    for (auto x : second)
        S.erase(S.find(x));
    first = vector< pair<int, int> >(S.begin(), S.end());

    cout << power << "\n";
    cout << first.size() << "\n";
    for (auto x : first)
        cout << x.second << " ";
    cout << "\n";

    cout << second.size() << "\n";
    for (auto x : second)
        cout << x.second << " ";
    cout << "\n";
}

