#include <iostream>
#include <fstream>
#include <cassert>
#include <deque>
#include <vector>

using namespace std;

int main() {
    ifstream cin("permsplit.in");
    ofstream cout("permsplit.out");

    int N; cin >> N;
    assert(2 <= N and N <= 1000000);

    deque<int> D(N);
    vector<int> P(N, 0);
    for (int i = 0; i < N; ++i) {
        cin >> D[i];
        assert(1 <= D[i] and D[i] <= N);
        P[D[i] - 1] = 1;
    }
    assert(P == vector<int>(N, 1));

    vector<int> answer;
    int left = 0;
    int right = 0;
    int minimum = 1, maximum = N;
    while (D.size()) {
        int value;
        if (D.front() == minimum || D.front() == maximum) {
            answer.push_back(++left);

            value = D.front();
            D.pop_front();
        } else if (D.back() == minimum || D.back() == maximum) {
            answer.push_back(N - (++right));

            value = D.back();
            D.pop_back();
        } else {
            cout << "-1\n";
            return 0;
        }

        if (value == minimum)
            ++minimum;
        else
            --maximum;
    }

    answer.pop_back();

    for (auto &x : answer)
        cout << x << " ";
    cout << "\n";
}
