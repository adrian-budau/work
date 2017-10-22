#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

vector<int64_t> solve_left(const string& S) {
    int N = S.size();
    vector<int64_t> answer(N, 0);
    vector< pair<int, int> > diffs(1, make_pair(N, 0));
    for (int i = 0; i < N; ) {
        if (S[i] == '.') {
            answer[i] = 0;
            diffs.back().first++;
            ++i;
            continue;
        }
        if (diffs.back().first == 0)
            diffs.pop_back();
        diffs.back().second++;
        int j;
        for (j = i; j < N && S[j] == S[i]; ++j);
        int aux = diffs.back().second, pos = diffs.size() - 1, count = 0;
        auto next = [&]() {
            int answer = aux;
            if (++count == diffs[pos].first) {
                --pos;
                count = 0;
                aux += diffs[pos].second;
            }
            return answer;
        };
        int64_t so_far = 0;
        for (--j; j >= i; --j) {
            so_far += next();
            answer[j] = so_far;
        }
        for (j = i; j < N && S[j] == S[i]; ++j);
        i = j;
        if (diffs.back().first != 0)
            diffs.emplace_back(0, 0);
    }
    return answer;
}

int main() {
    ifstream cin("slide.in");
    ofstream cout("slide.out");

    string S; assert(cin >> S);
    int N = S.size();
    assert(1 <= N && N <= 1000 * 1000);

    auto answerLeft = solve_left(S);
    reverse(S.begin(), S.end());
    auto answerRight = solve_left(S);
    reverse(answerRight.begin(), answerRight.end());

    int64_t answer = 0;
    for (int i = 0; i < N; ++i)
        answer = (answer + min(answerLeft[i], answerRight[i]));
    cout << answer << "\n";
}
