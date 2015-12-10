#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    ifstream cin("kbetray.in");
    ofstream cout("kbetray.out");

    int N, K; cin >> N >> K;
    vector< pair<int, int> > A(N);
    vector<int> B(2 * N);
    int64_t answer = 0;
    for (int i = 0; i < N; ++i) {
        cin >> A[i].first >> A[i].second;
        answer += max(A[i].first, A[i].second);
        B[i] = A[i].first;
        B[i + N] = A[i].second;
    }

    nth_element(B.begin(), B.begin() + N, B.end());
    int middle = B[N];
    vector<int> caseBig, caseSmall;
    for (auto &p : A) {
        if (p.first < p.second)
            swap(p.first, p.second);
        if (p.second >= middle)
            caseBig.push_back(p.second);
        else if (p.first < middle)
            caseSmall.push_back(p.first);
    }

    while (caseSmall.size() < caseBig.size())
        caseSmall.push_back(middle);

    sort(caseBig.begin(), caseBig.end(), greater<int>());
    sort(caseSmall.begin(), caseSmall.end());

    for (int j = 0; j < int(caseBig.size()) && j < int(caseSmall.size()) && j < K; ++j)
        answer += caseBig[j] - caseSmall[j];

    cout << answer << "\n";
}
