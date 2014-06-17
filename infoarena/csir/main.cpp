#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>

using namespace std;

bool cString(string A) {
    if (count(A.begin(), A.end(), 'A') < count(A.begin(), A.end(), 'B'))
        for (auto &c : A)
            c = 'A' + 'B' - c;

    vector<int> sizes;
    int last = -1;

    for (int i = 0; i < int(A.size()); ++i) {
        if (A[i] == 'B') {
            if (last == i - 1 and last != -1)
                return false;
            else {
                if (last != i - 1)
                    sizes.push_back(i - last - 1);
                last = i;
            }
        }
    }

    if (last != int(A.size()) - 1)
        sizes.push_back(A.size() - last - 1);

    if (A.front() == A.back()) {
        if (A.front() == 'B')
            return false;
        else if (count(A.begin(), A.end(), 'B')) { // otherwise do nothing, it's okay
            sizes.front() += sizes.back();
            sizes.pop_back();
        }
    }

    int mint = *min_element(sizes.begin(), sizes.end()),
        maxt = *max_element(sizes.begin(), sizes.end());

    if (mint == maxt)
        return true;

    if (maxt - mint > 1)
        return false;
    string newA(sizes.size(), 'a');
    for (int i = 0; i < int(sizes.size()); ++i)
        if (sizes[i] == mint)
            newA[i] = 'A';
        else
            newA[i] = 'B';
    return cString(newA);
}

int main() {
    ifstream cin("csir.in");
    ofstream cout("csir.out");

    int T; cin >> T;
    while (T--) {
        string A; cin >> A;

        if (cString(A))
            cout << "1\n";
        else
            cout << "0\n";
    }
}
