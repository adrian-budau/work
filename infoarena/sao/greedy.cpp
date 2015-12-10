#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    ifstream cin("sao.in");
    ofstream cout("sao.out");

    int N, M; int64_t K; cin >> N >> M >> K;

    vector<int> hp(N);
    for (int i = 0; i < N; ++i)
        cin >> hp[i];

    int step;
    for (step = 1; step < N; step <<= 1);
    int answer = 0;
    vector<pair<int, int>> hp2(N);
    vector<bool> used(N);
    for (answer = M; step; step >>= 1) {
        if (answer + step > N)
            continue;
        answer += step;
        for (int i = 0; i < answer; ++i) {
            hp2[i] = {hp[i], -i};
            used[i] = true;
        }

        nth_element(hp2.begin(), hp2.begin() + answer - M, hp2.begin() + answer);
        for (int i = answer - M; i < answer; ++i)
            used[-hp2[i].second] = false;

        int64_t aux = K;
        int i;
        for (i = 0; i < answer && aux > 0; ++i) {
            if (used[i]) {
                if (aux < hp[i])
                    break;
                aux -= hp[i];
            }
        }

        if (i < answer)
            answer -= step;
    }

    cout << answer << "\n";
}
