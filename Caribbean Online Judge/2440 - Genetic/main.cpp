#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

int main() {
    int T; cin >> T;

    while (T--) {
        string M, N; cin >> M >> N;

        if (count(M.begin(), M.end(), 'T') > 0 and M.find(N) != M.npos)
            cout << "Y\n";
        else
            cout << "N\n";
    }
}
