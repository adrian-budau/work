#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <cassert>
#include <cmath>

using namespace std;

int main() {
    ifstream cin("sao.in");
    ofstream cout("sao.out");

    int N, M; int64_t K; assert(cin >> N >> M >> K);
    assert(1 <= N && N <= 100 * 1000);
    assert(0 <= M && M <= N);
    assert(1 <= K && K <= 1000 * 1000 * 1000);

    vector<int> hp(N);
    for (int i = 0; i < N; ++i) {
        assert(cin >> hp[i]);
        assert(1 <= abs(hp[i]) && abs(hp[i]) <= 1000 * 1000 * 1000);
    }

    int i;
    priority_queue<int> P;
    for (i = 0; i < N; ++i) {
        K -= hp[i];
        P.push(hp[i]);
        if (K <= 0) {
            if (M == 0) {
                if (K == 0)
                    ++i;
                break;
            }
            K += P.top();
            P.pop();
            --M;
        }
    }

    cout << i << "\n";
}
