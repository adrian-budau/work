#include <cstdio>
#include <tr1/unordered_map>
#include <vector>
#include <algorithm>
#include <cassert>
#include <iostream>

using namespace std;
using namespace tr1;

long long cmmdc(long long a, long long b) {
    if (b == 0)
        return a;
    return cmmdc(b, a % b);
}

const int maxim = 1000000000;

long long sqrt2(long long K) {
    long long step, i;
    for (step = 1; step < K; step <<= 1);
    for (i = 0; step; step >>= 1)
        if (i + step <= maxim && (i + step) * (i + step) <= K)
            i += step;
    return i;
}

void verif(long long &a) {
    if (a < 1 || a > maxim)
        a = -1;
}

void get(long long &a1, long long &a2, long long &a3, long long a1a2, long long a1a3, long long a2a3) {
    long long c = cmmdc(a1a2, a1a3);
    long long a2sk = a1a2 / c;
    long long a3sk = a1a3 / c;
    if (a2sk > maxim || a3sk > maxim) {
        a1 = a2 = a3 = -1;
        return;
    }

    long long a2a3sk2 = a2sk * a3sk;
    if (a2a3 % a2a3sk2) {
        a1 = a2 = a3 = -1;
        return;
    }

    long long k = a2a3 / a2a3sk2;
    long long k2 = sqrt2(k);
    if (k2 * k2 != k) {
        a1 = a2 = a3 = -1;
        return;
    }

    k = k2;

    if (c % k) {
        a1 = a2 = a3 = -1;
        return;
    }

    a1 = c / k;
    if (a1a2 % a1 or a1a3 % a1) {
        a1 = a2 = a3 = -1;
        return;
    }
    a2 = a1a2 / a1;
    a3 = a1a3 / a1;
}

int main() {
    cin.sync_with_stdio(false);
    int T; cin >> T;
    for (int i = 1; i <= T; ++i) {
        int N; cin >> N;
        int M = N * (N - 1) / 2;
        vector<long long> A(M);
        for (int i = 0; i < M; ++i)
            cin >> A[i];
        sort(A.begin(), A.end());

        vector<long long> answer(N, maxim + 1);
        for (int i = 2; i < N; ++i) {
            long long a1, a2, a3;
            get(a1, a2, a3, A[0], A[1], A[i]);
            verif(a1);
            verif(a2);
            verif(a3);
            if (a1 == -1)
                continue;

            unordered_map<long long, int> MT;
            MT[A[0]]++;
            MT[A[1]]++;
            MT[A[i]]++;

            int pos = 3;
            vector<long long> aux(N);
            aux[0] = a1;
            aux[1] = a2;
            aux[2] = a3;
            for (int j = 0; j < M; ++j)
                if (MT[A[j]] > 0)
                    --MT[A[j]];
                else {
                    if (pos == N) {
                        pos = -1;
                        break;
                    }

                    if (A[j] % aux[0])
                        break;
                    aux[pos] = A[j] / aux[0];
                    verif(aux[pos]);
                    if (aux[pos] == -1) {
                        pos = -1;
                        break;
                    }
                    for (int k = 1; k < pos; ++k)
                        MT[1LL * aux[k] * aux[pos]]++;
                    ++pos;
                }
            if (pos == N) {
                //for (int i = 1; i < N; ++i)
                //    if (aux[i - 1] == aux[i])
                //        aux = vector<long long>(N, maxim + 1);
                if (aux < answer)
                    answer = aux;
            }
        }

        assert(answer[0] != maxim + 1);
        cout << "Case " << i << ": ";
        for (int i = 0; i < N; ++i) {
            cout << answer[i];
            if (i < N - 1)
                cout << " ";
        }
        cout << "\n";
    }
}
