#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
 
using namespace std;
 
int gcd(const int &a, const int &b) {
    if (b == 0)
        return a;
    return gcd(b, a % b);
}
 
class functor {
  public:
    bool operator()(const pair<int, int> &A, const pair<int, int> &B) const {
        return A.first * B.second < A.second * B.first;
    }
};
 
int solve(int N, int answer) {
    vector<int> many(N + 1);
    int sum = 0;
    for (int j = 1; j <= N; ++j) {
        many[j] += (answer * j - 1) / N;
 
        for (int k = j + j; k <= N; k += j)
            many[k] -= many[j];
        sum += many[j];
    }
    return sum;
}
 
int main() {
    ifstream cin("farey.in");
    ofstream cout("farey.out");
 
    int N; cin >> N;
    int K; cin >> K;
 
    int step, answer;
    for (step = 1; step < N; step <<= 1);
    step >>= 1;
 
    for (answer = 0; step; step >>= 1) {
        if (answer + step >= N)
            continue;
 
        answer += step;
 
        vector<int> many(N + 1, 0);
         
        if (solve(N, answer) >= K)
            answer -= step;
    }
 
    ++answer;
 
    K -= solve(N, answer - 1) + 1;
 
    cerr << answer << " " << K << "\n";
    vector< pair<int, int> > A;
    for (int i = 1; i <= N; ++i) {
        int element = (answer * i - 1) / N;
        if (element * N >= (answer - 1) * i && element * N < answer * i)
            if (gcd(i, element) == 1)
                A.push_back(make_pair(element, i));
    }
 
    sort(A.begin(), A.end(), functor());
 
    cout << A[K].first << " " << A[K].second << "\n";
}
