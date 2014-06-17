#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
 
using namespace std;
 
long double timeSwap(pair<int, int> &A, pair<int, int> &B, vector<long long> &dp) {
    return static_cast<long double>(dp[A.second] - dp[B.second]) / static_cast<long double>(A.first - B.first);
}
 
int main() {
    ifstream cin("euro.in");
    ofstream cout("euro.out");
 
    int N, T; cin >> N >> T;
 
    // It's no use to have an element positive unless it's the last element
    int currentSum = 0;
    vector< pair<int, int> > A;
    A.push_back(make_pair(0, 0));
 
    for (int i = 0; i < N; ++i) {
        int value; cin >> value;
 
        currentSum += value;
 
        if (currentSum < 0) {
            A.push_back(make_pair(currentSum, i + 1));
            currentSum = 0;
        }
    }
 
    if (A.size() == 0 || A.back().second != N)
        A.push_back(make_pair(currentSum, N));
 
    vector<long long> dp(N + 1, 0);
    N = A.size() - 1;
 
    for (int i = 1; i <= N; ++i)
        A[i].first += A[i - 1].first;
 
     
    vector< pair<int, int> > stack;
    int from = 0, to = 0;
 
    for (int i = 1; i <= N; ++i) {
        //cerr << A[i].first << " " << A[i].second << "\n";
        while (to - from > 1 && timeSwap(stack[to - 2], stack[to - 1], dp) + 1e-9 > timeSwap(stack[to - 1], A[i - 1], dp))
            stack.pop_back(), --to;
 
        stack.push_back(A[i - 1]);
        ++to;
 
        while (to - from > 1) {
            //cerr << "Checking (" << stack[from].first << ", " << stack[from].second << ") with (" << stack[from + 1].first << ", " << stack[from + 1].second << ")  ";
            //cerr << timeSwap(stack[from], stack[from + 1], dp) << "\n";
            if (timeSwap(stack[from], stack[from + 1], dp) <= A[i].second + 1e-9)
                ++from;
            else
                break;
        }
 
        dp[A[i].second] = static_cast<long long>(A[i].first) * A[i].second - static_cast<long long>(A[i].second) * stack[from].first + dp[stack[from].second] - T;
        //cerr << A[i].second << " -> " << dp[A[i].second] << "     " << from << " -> " << to << "\n\n";
    }
 
    cout << dp[A.back().second] << "\n";
}
