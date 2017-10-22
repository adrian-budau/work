#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

static const int kModulo = 1000 * 1000 * 1000 + 7;

void reverse(pair<int, int>& p, int N) {
    p.first = N - p.first - 1;
    p.second = N - p.second - 1;
    swap(p.first, p.second);
}

vector<int> solve_left(const vector< pair<int, int> > &V, int N) {
    vector<int> answer(N, 0);
    for (auto &p : V) {
        for (int i = p.first ; i <= p.second; ++i) {
            int left = i - (p.second - p.first + 1);
            if (left >= 0)
                answer[i] += answer[left];
            answer[i] += (p.second - i + 1);
        }
    }

    return answer;
}

int main() {
    ifstream cin("slide.in");
    ofstream cout("slide.out");

    string S; assert(cin >> S);
    int N = S.size();
    assert(1 <= N && N <= 1000 * 1000);
    vector< pair<int, int> > V;
    for (int i = 0; i < N; ++i)
        if (S[i] != '.') {
            int j;
            for (j = i; j < N && S[j] == S[i]; ++j);
            V.emplace_back(i, j - 1);
            i = j - 1;
        }

    auto answerLeft = solve_left(V, N);
    for (auto &p : V)
        reverse(p, N);
    reverse(V.begin(), V.end());
    auto answerRight = solve_left(V, N);
    reverse(answerRight.begin(), answerRight.end());

    int answer = 0;
    for (int i = 0; i < N; ++i)
        answer = (answer + min(answerLeft[i], answerRight[i])) % kModulo;
    cout << answer << "\n";
}
