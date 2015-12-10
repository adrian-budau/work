#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>

using namespace std;

int main() {
    ifstream cin("prefix2.in");
    ofstream cout("prefix2.out");

    string S; cin >> S;

    int N = S.size();

    map<string, int> M;

    int answer = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j <= i; ++j)
            if (++M[S.substr(j, i - j + 1)] == 1)
                ++answer;
        cout << answer << "\n";
    }
}
