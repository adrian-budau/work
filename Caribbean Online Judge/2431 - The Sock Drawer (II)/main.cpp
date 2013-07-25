#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main() {
    int T; cin >> T;

    for (int test = 1; test <= T; ++test) {
        int K; cin >> K;

        vector<int> A(K);
        int total = 0;
        for (int i = 0; i < K; ++i) {
            cin >> A[i];
            total += A[i];
        }

        long double answer = 0;

        for (int i = 1; i < (1 << K); ++i) {
            int biti = 0;
            for (int j = 0; j < K; ++j)
                if ((1 << j) & i)
                    ++biti;
            long double aux = 1;
            for (int i = 1; i <= biti + 1; ++i)
                aux *= i;
            int sum = 0;
            for (int j = 0; j < K; ++j)
                if ((1 << j) & i) {
                    aux *= A[j];
                    sum += A[j] - 1;
                }
            aux *= sum;
            for (int j = total - biti; j <= total; ++j)
                aux /= j;
            answer += aux;
        }

        cout << "Case " << test << ": ";
        cout.setf(ios::fixed, ios::floatfield);
        cout.precision(3);
        cout << answer << "\n";
    }
}
