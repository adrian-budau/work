#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <algorithm>

using namespace std;

typedef array< array<short, 2>, 2> state;

string::iterator Begin, End;

state series();
state parallel();
state term();

state parse() {
    return series();
}

state series() {
    auto answer = parallel();
    while (Begin < End && *Begin == 'S') {
        ++Begin;
        auto currentAnswer = parallel();
        state nextAnswer({});
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 2; ++j)
                nextAnswer[i][j] = min(answer[i][0] + currentAnswer[0][j], answer[i][1] + 1 + currentAnswer[1][j]);
        swap(answer, nextAnswer);
    }
    return answer;
}

state parallel() {
    auto answer = term();
    while (Begin < End && *Begin == 'P') {
        ++Begin;
        auto currentAnswer = term();
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 2; ++j)
                answer[i][j] += currentAnswer[i][j];
    }
    return answer;
}

state term() {
    if (*Begin == '(') {
        ++Begin;
        auto answer = series();
        ++Begin;
        return answer;
    }
    ++Begin;
    state answer;
    answer[0][1] = answer[1][0] = answer[1][1] = 0;
    answer[0][0] = 1;
    return answer;
}

int main() {
    ifstream cin("rsp.in");
    ofstream cout("rsp.out");

    int T; cin >> T;
    while (T--) {
        string S; cin >> S;
        Begin = S.begin();
        End = S.end();
        auto s = parse();
        cout << min<int>({s[0][0], s[0][1] + 1, s[1][0] + 1, s[1][1] + 2}) << "\n";
    }
}
