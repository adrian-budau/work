#include <fstream>
#include <map>
#include <string>

using namespace std;

int main() {
    ifstream cin("pufu.in");
    ofstream cout("pufu.out");

    int T; cin >> T;
    while (T--) {
        int N; cin >> N;
        map<string, int> M;

        for (int i = 0; i < N; ++i) {
            string S; cin >> S;
            M[S]++;
        }

        cout << M["ciocolata"] << " " << M["cascaval"] << " " << M["sare"] << "\n";
    }
}
