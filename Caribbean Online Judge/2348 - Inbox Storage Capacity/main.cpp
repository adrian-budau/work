#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

int main() {
    int T; cin >> T;

    while (T--) {
        int N, C; cin >> N >> C;

        vector< pair<int, int> > events;
        for (int i = 0; i < N; ++i) {
            int a, b, c;
            cin >> a >> b >> c;
            events.push_back(make_pair(a, c));
            events.push_back(make_pair(b, -c));
        }

        long long total = 0;
        long long most = 0;
        sort(events.begin(), events.end());
        for (vector< pair<int, int> >::iterator it = events.begin(); it != events.end(); ++it) {
            total += it -> second;
            most = max(most, total);
        }

        if (most > C)
            cout << "BLOCKED" << " " << most - C << "\n";
        else if (most * 10 > 9 * C)
            cout << "OVERLOAD" << " " << (most * 10 + 8) / 9 - C << "\n";
        else
            cout << "OK\n";
    }
}
