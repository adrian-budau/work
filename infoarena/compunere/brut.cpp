#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;
 
int main() {
    ifstream cin("compunere.in");
    ofstream cout("compunere.out");
 
    int n, m; cin >> n >> m;
    vector<int> A(n, 0), B(m, 0);
    for(int i = 0; i < n; ++i)
        cin >> A[i];
    for(int i = 0; i < m; ++i)
        cin >> B[i];
     
    reverse(A.begin(), A.end());
    reverse(B.begin(), B.end());
 
    vector<vector<int>> dp(n + 1, vector<int> (m + 1, 0));
 
    for(int i = 1; i <= n; ++i)
        for(int j = 1; j <= m; ++j) {
            dp[i][j] = max(dp[i][j], dp[i - 1][j]);
            dp[i][j] = max(dp[i][j], dp[i][j - 1]);
            dp[i][j] = max(dp[i][j], dp[i - 1][j - 1] + (A[i - 1] == B[j - 1]));
        }
     
    int sol = dp[n][m];
     
    vector<int> ans;
    int it1 = n, it2 = m;
 
    while(it1 != 0 || it2 != 0) {
        //cerr << it1 << " " << it2 << " " << sol << "\n";
        if(it1 && it2 && A[it1 - 1] == B[it2 - 1]) {
            ans.push_back(A[it1 - 1]);
            it1--, it2--;
            --sol;
        } else if(it1 && !it2) {
            while(it1) {
                ans.push_back(A[it1 - 1]);
                it1--;
            }
        } else if(it2 && !it1) {
            while(it2) {
                ans.push_back(B[it2 - 1]);
                it2--;
            }
        } else {
            if(A[it1 - 1] < B[it2 - 1]) {
                if(dp[it1 - 1][it2] == sol) {
                    ans.push_back(A[it1 - 1]);
                    it1--;
                } else {
                    ans.push_back(B[it2 - 1]);
                    it2--;
                }
            } else {
                if(dp[it1][it2 - 1] == sol) {
                    ans.push_back(B[it2 - 1]);
                    it2--;
                } else {
                    ans.push_back(A[it1 - 1]);
                    it1--;
                }
            }
        }
    }
     
    cout << ans.size() << "\n";
    for(auto tmp : ans)
        cout << tmp << " ";
}
