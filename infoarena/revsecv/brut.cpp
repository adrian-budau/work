#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int main() {
    ifstream cin("revsecv.in");
    ofstream cout("revsecv.out");

    string S; cin >> S;

    int N = S.size();
    int64_t answer = 0;
    for (int i = 0; i < N; ++i) {
        int iorig = i;
        for (int j = i + 1, k; i >= 0 && j < N; i -= k + 1, j += k + 1) {
            for (k = 0; i - k >= 0 && j + k < N && S[i - k] == S[j + k]; ++k);
            answer += 1LL * k * (k + 1) / 2;
        }
        i = iorig;
        for (int j = i + 2, k; i >= 0 && j < N; i -= k + 1, j += k + 1) {
            for (k = 0; i - k >= 0 && j + k < N && S[i - k] == S[j + k]; ++k);
            answer += 1LL * k * (k + 1) / 2;
        }
        i = iorig;
    }

    cout << answer << "\n";
}
