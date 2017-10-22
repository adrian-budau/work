#include <iostream>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    int T; cin >> T;

    int test = 0;
    while (T--) {
        int N; cin >> N;
        string S; cin >> S;
        cout << "Case #" << ++test << ":\n";
        if (N == 1) {
            cout << 1 << "\n";
            continue;
        }

        int Aodd = 0, Aeven = 0, Bodd = 0, Beven = 0;
        for (int i = 0; i < int(S.size()); ++i) {
            if (S[i] == 'A')
                if (i % 2)
                    Aodd = 1;
                else
                    Aeven = 1;
            else
                if (i % 2)
                    Bodd = 1;
                else
                    Beven = 1;
        }

        if ((Aodd + Aeven == 1 and Bodd + Beven == 1) or Bodd + Beven == 0 or Aodd + Aeven == 0) {
            cout << 2 << "\n";
            continue;
        }

        if (Aodd + Aeven == 1 or Bodd + Beven == 1) {
            cout << 3 << "\n";
            continue;
        }
        cout << 4 << "\n";
    }
}

