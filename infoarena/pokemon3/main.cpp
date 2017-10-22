#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

int main() {
    ifstream cin("pokemon3.in");
    ofstream cout("pokemon3.out");

    int N, M; assert(cin >> N >> M);
    assert(1 <= N && N <= 20);
    assert(1 <= M && M <= 10 * 1000);

    vector<int> beats(N, 0);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int x; assert(cin >> x);
            assert(0 <= x && x <= 1);
            if (x == 1) {
                beats[i] |= (1 << j);
                assert((beats[j] & (1 << i)) == 0);
            }
        }
    }

    int enemy = 0;
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < 3; ++j) {
            int x; assert(cin >> x);
            enemy |= (1 << (x - 1));
        }
    }

    int answer = N + 1;
    for (int i = 0; i < (1 << N); ++i) {
        int power = 0;
        for (int j = 0; j < N; ++j)
            if ((1 << j) & i)
                power |= beats[j];
        if ((power & enemy) == enemy)
            answer = min(answer, __builtin_popcount(i));
    }

    if (answer > N)
        answer = -1;
    cout << answer << "\n";
}

