#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

vector<int> decompose(int64_t number, int many) {
    int count = 0;
    for (int i = 0; i < 60; ++i)
        if ((1LL << i) & number)
            ++count;
    if (count > many)
        return vector<int>();

    vector<int> how;
    for (int i = 0; i < 60; ++i)
        if ((1LL << i) & number)
            how.push_back(i);
    for (int i = 0; i < int(how.size()); ++i)
        while (int(how.size()) < many && how[i] > 0) {
            how.push_back(how[i] - 1);
            --how[i];
        }
    return how;
}
int main() {
    ios::sync_with_stdio(false);
    int T; cin >> T;

    while (T--) {
        int64_t N; cin >> N;
        for (int i = 1; i <= 64; ++i) {
            auto how = decompose(N * i, i);
            if (how.size() == 0)
                continue;
            cout << how.size() << " ";
            for (auto &x : how)
                cout << x << " ";
            cout << "\n";
            break;
        }
    }
}
