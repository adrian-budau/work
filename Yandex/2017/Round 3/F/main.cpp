// For RED -> N - (N / 2) - (N - 1 - R-R edges)
// FOR BLUE -> N / 2 - (N - 1 - B-B edges)
//
// score N % 2 + R-R edges - B-B edges

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    int N; cin >> N;

    vector<int> degree(N);
    for (int i = 1; i < N; ++i) {
        int x, y; cin >> x >> y;
        --x; --y;
        degree[x]++;
        degree[y]++;
    }

    int answer = 0;
    sort(degree.begin(), degree.end());
    for (int i = 0; i < N; ++i)
        if (i % 2)
            answer += degree[i];
        else
            answer -= degree[i];
    answer /= 2;
    answer += N % 2;
    cout << answer << "\n";
}
