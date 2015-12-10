#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <set>

using namespace std;

int main() {
    ifstream cin("sirag.in");
    ofstream cout("sirag.out");

    int K, P; cin >> K >> P;
    vector<int> A(K);

    int64_t sum = 0;
    for (int i = 0; i < K; ++i) {
        cin >> A[i];
        sum += A[i];
    }
    sort(A.begin(), A.end(), greater<int>());

    int64_t step;
    for (step = 1; step < sum; step *= 2);
    int64_t answer;
    for (answer = 0; step; step /= 2) {
        answer += step;

        int column = 0;
        int64_t need = answer / P + int(answer % P > column);

        for (int i = 0; i < K; ++i) {
            int64_t have = min<int64_t>(A[i], answer / P + int(answer % P > i));
            if (have < need) {
                need -= have;
                continue;
            }

            have -= need;
            ++column;
            need = answer / P + int(answer % P > column);
            need -= have;
        }

        if (column < P)
            answer -= step;
    }

    cout << answer << "\n";
}
