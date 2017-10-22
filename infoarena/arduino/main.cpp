#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

void back(int start, int end, int maxv, const vector< pair<int, vector<int> > > &restrictions, vector<int>& mask, vector<int>& count) {
    if (start == end) {
        for (auto &r : restrictions) {
            int need = r.first;
            int have = 0;
            for (auto &x : r.second)
                have |= (1 << mask[x]);
            if (__builtin_popcount(have) != need)
                return;
        }
        count[maxv + 1]++;
        return;
    }

    for (int i = 0; i <= maxv; ++i) {
        mask[start] = i;
        back(start + 1, end, maxv, restrictions, mask, count);
    }
    if (start != 0) {
        mask[start] = maxv + 1;
        back(start + 1, end, maxv + 1, restrictions, mask, count);
    }
}

static const int kModulo = 1000 * 1000 * 1000 + 7;

int main() {
    ifstream cin("arduino.in");
    ofstream cout("arduino.out");

    int N, M; cin >> N >> M;
    int R; cin >> R;
    vector< pair<int, vector<int> > > restrictions(R);
    for (int i = 0; i < R; ++i) {
        int K; cin >> restrictions[i].first >> K;
        for (int j = 0; j < K; ++j) {
            int x; cin >> x;
            --x;
            restrictions[i].second.push_back(x);
        }
    }

    vector<int> count(N + 1, 0);
    vector<int> mask(N);
    back(0, N, 0, restrictions, mask, count);

    int comb = 1, answer = 0;
    for (int i = 1; i <= N; ++i) {
        comb = 1LL * comb * (M - i + 1) % kModulo;
        int temp = comb;
        temp = 1LL * temp * count[i] % kModulo;
        answer = (answer + temp) % kModulo;
    }

    cout << answer << "\n";
}
