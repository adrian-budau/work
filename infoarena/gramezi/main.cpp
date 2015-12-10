#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    ifstream cin("gramezi.in");
    ofstream cout("gramezi.out");

    int N; cin >> N;
    vector< pair<int, int> > A(N);
    for (int i = 0; i < N; ++i)
        cin >> A[i].first >> A[i].second;


    sort(A.begin(), A.end(), [&](pair<int, int> a, pair<int, int> b) {
        if (a.first + a.second != b.first + b.second)
            return a.first + a.second > b.first + b.second;
        return a.first > b.first;
    });

    bool firstMove = true;
    int firstScore = 0, secondScore = 0;
    for (int i = 0; i < N;) {
        int j;
        for (j = i; j < N && A[j].first + A[j].second == A[i].first + A[i].second; ++j);
        for (int k = i; k < j; ++k)
            if (k < i + (j - i + int(firstMove)) / 2)
                firstScore += A[k].first;
            else
                secondScore += A[k].second;
        if ((j - i) % 2)
            firstMove = !firstMove;
        i = j;
    }

    cout << firstScore << " " << secondScore << "\n";
}
