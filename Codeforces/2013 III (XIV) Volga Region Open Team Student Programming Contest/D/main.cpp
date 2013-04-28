#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main() {
    ifstream cin("input.txt");
    ofstream cout("output.txt");
    int D, N, M; cin >> D >> N >> M;

    vector<long long> V(N, 0);
    for (int i = 0; i < D; ++i)
        for (int j = 0; j < N; ++j) {
            int x; cin >> x;
            V[j] += x;
        }

    long long answer = -1;
    int where = 0;
    for (int i = 0; i < N; ++i) {
        if (i + M > N)
            break;

        long long tmp = 0;
        for (int j = i; j < i + M; ++j)
            tmp += V[j];
        if (tmp > answer) {
            answer = tmp;
            where = i + 1;
        }
    }

    cout << where << " " << answer << "\n";
}
