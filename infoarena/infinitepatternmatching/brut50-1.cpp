#include <iostream>
#include <fstream>

using namespace std;

int main() {
    ifstream cin("infinitepatternmatching.in");
    ofstream cout("infinitepatternmatching.out");

    string S; cin >> S;

    string text;
    for (int i = 1; i < 300000; ++i) {
        int j;
        for (j = 0; (1 << j) <= i; ++j);
        for (--j; j >= 0; --j)
            if ((1 << j) & i)
                text += '1';
            else
                text += '0';
    }

    cout << text.find(S) + S.size() << "\n";
}
