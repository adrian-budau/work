#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    int N; cin >> N;

    vector<int> plus;
    vector<int> minus;
    for (int i = 0; i < N; ++i) {
        int x, y; cin >> x >> y;
        if (x > 0)
            plus.push_back(-y);
        else
            minus.push_back(-y);
    }

    sort(plus.begin(), plus.end());
    sort(minus.begin(), minus.end());

    int M; cin >> M;
    for (int i = 0; i < M; ++i) {
        int x; cin >> x;
        int answer = 0;
        answer += plus.end() - lower_bound(plus.begin(), plus.end(), -x, [&](int a, int b) {
            return a < b;
        });
        answer += minus.end() - lower_bound(minus.begin(), minus.end(), x, [&](int a, int b) {
            return a < b;
        });

        cout << answer << "\n";
    }
}
