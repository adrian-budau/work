#include <iostream>
#include <fstream>
#include <vector>
#include <array>

using namespace std;

int main() {
    ifstream cin("rsa.in");
    ofstream cout("rsa.out");

    int T, M; cin >> T >> M;

    vector<int> primes{2};
    for (int i = 1; i < T; ++i) {
        for (int j = primes.back() + 1; ; ++j) {
            bool ok = true;
            for (auto &x : primes)
                if (j % x == 0) {
                    ok = false;
                    break;
                }
            if (ok) {
                primes.push_back(j);
                break;
            }
        }
    }

    vector< vector<int> > mat(T, vector<int>(M, 0));
    for (int i = 0; i < M; ++i) {
        int b; cin >> b;
        for (int j = 0; j < T; ++j) {
            int v = 0;
            while (b % primes[j] == 0) {
                b /= primes[j];
                ++v;
            }
            if (v % 2)
                mat[j][i] = 1;
        }
    }

    auto xorm = [&](vector<int> a, vector<int> b) {
        vector<int> c(a.size());
        for (int i = 0; i < int(a.size()); ++i)
            c[i] = a[i] ^ b[i];
        return c;
    };

    int fixed = 0;
    for (int i = 0, j = 0; i < T and j < M; ++j) {
        if (mat[i][j] == 0)
            for (int k = i + 1; k < T; ++k)
                if (mat[k][j] == 1) {
                    swap(mat[i], mat[k]);
                    break;
                }
        if (!mat[i][j])
            continue;

        ++fixed;
        for (int k = i + 1; k < T; ++k)
            if (mat[k][j] == 1)
                mat[k] = xorm(mat[k], mat[i]);
        ++i;
    }

    int answer = M - fixed;
    array<int, 100> nr = {1, 1};
    for (int i = 0; i < answer; ++i) {
        int rest = 0, j;
        for (j = 1; j <= nr[0] || rest; ++j, rest /= 10) {
            rest = nr[j] * 2 + rest;
            nr[j] = rest % 10;
        }

        nr[0] = j - 1;
    }

    --nr[1];
    for (int i = nr[0]; i > 0; --i)
        cout << nr[i];
    cout << "\n";
}
