#include <iostream>
#include <algorithm>

using namespace std;

int main() {
    auto reverse = [&](string S) {
        ::reverse(S.begin(), S.end());
        return S;
    };

    int T; cin >> T;
    while (T--) {
        string S; cin >> S;
        cout << reverse(S) << "\n";
    }
}
