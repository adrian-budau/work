#include <iostream>
#include <fstream>
#include <set>

using namespace std;

int main() {
    int T; cin >> T;

    while (T--) {
        string A, B; cin >> A >> B;

        set<char> S;
        for (auto &x : A)
            S.insert(x);

        bool ok =false;
        for (auto &x : B)
            if (S.count(x))
                ok = true;
        if (ok)
            cout << "YES\n";
        else
            cout << "NO\n";
    }
}
