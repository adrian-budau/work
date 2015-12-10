#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    ifstream cin("tdeque.in");
    ofstream cout("tdeque.out");
    int N; cin >> N;

    string ops;

    vector<int> arr;
    while (N--) {
        int x; cin >> x;
        if (arr.empty()) {
            arr.push_back(x);
            ops += "1";
            continue;
        }

        int where = min_element(arr.begin(), arr.end()) - arr.begin();
        for (int i = 0; i < int(arr.size()); ++i)
            if (arr[i] > x)
                if (arr[where] < x || arr[where] > arr[i])
                    where = i;

        rotate(arr.begin(), arr.begin() + where, arr.end());
        if (where <= int(arr.size()) - where) {
            ops += string(where, '3');
        } else {
            ops += string(arr.size() - where, '2');
        }

        arr.push_back(x);
        ops += "1";
    }

    auto where = min_element(arr.begin(), arr.end());
    rotate(arr.begin(), where, arr.end());

    if (where - arr.begin() <= arr.end() - where)
        ops += string(where - arr.begin(), '3');
    else
        ops += string(arr.end() - where, '2');

    cout << ops.size() << "\n";
    cout << ops << "\n";
}
