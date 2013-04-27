#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <cmath>

using namespace std;

const int maxn = 1000001;
long long a[maxn];
unordered_map<int, long long> A;

long long solve(int N) {
    if (N < maxn)
        return a[N];

    if (A.count(N))
        return A[N];

    long long M = N;
    long long answer = 1LL * (M + 1) * (M + 1) - 1;
    long long rad = sqrt(N);

    for (long long i = 2; i < rad + 1; ++i)
        answer -= solve(N / i);

    rad++;
    for (long long i = N / rad; i > 0; --i) {
        long long to = N / i;
        long long fr = N / (i + 1);

        answer -= 1LL * (to - fr) * solve(i);
    }

    //answer %= 10000;
    //if (answer < 0)
        //answer += 10000;
    A[N] = answer;
    return A[N];
}

int main() {

    freopen("cntgcd.in","r",stdin);
    freopen("cntgcd.out","w",stdout);
    
    cin.sync_with_stdio(false);

    for (int i = 1; i < maxn; ++i)
        a[i] = i;

    for (int i = 2; i < maxn; ++i)
        if (a[i] == i) {
            for (int j = i; j < maxn; j += i) {
                a[j] /= i;
                a[j] *= (i - 1);
            }
        }

    a[1] = 3;
    for (int i = 2; i < maxn; ++i) {
        a[i] = 2 * a[i] + a[i - 1];
        //a[i] %= 10000;
    }

    A[1] = 3;

    long long N, D; cin >> N >> D;

    cout << (solve(N / D) - 1) / 2 << "\n";
}

