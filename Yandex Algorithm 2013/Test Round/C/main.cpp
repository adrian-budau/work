#include <iostream>
#include <string>
#include <fstream>

using namespace std;

int main() {
    ifstream cin("number.in");
    ofstream cout("number.out");

    string S; cin >> S;

    int answer = 9999;
    for (int i = 0; i < 8; ++i)
        for (int j = i + 1; j < 8; ++j)
            for (int k = j + 1; k < 8; ++k)
                for (int p = k + 1; p < 8; ++p) {
                    int aux  = (S[i] - '0') * 1000 + (S[j] - '0') * 100 + (S[k] - '0') * 10 + S[p] - '0';
                    answer = min(answer, aux);
                }

    cout << answer << "\n";
}
