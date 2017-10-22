#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

vector<int> get_best(const vector< pair<int, int> >& V, int total) {
    vector<int> answer(total + 1, 0);
    int current_sum = 0;
    for (auto &p : V) {
        for (int i = current_sum; i >= 0; --i)
            answer[i + p.first + p.second] = max(answer[i + p.first + p.second], answer[i] + p.second);
        current_sum += p.first + p.second;
    }
    for (int i = 1; i <= total; ++i)
        answer[i] = max(answer[i], answer[i - 1]);
    return answer;
}

int main() {
    ios::sync_with_stdio(false);
    int N; cin >> N;
    vector< pair<int, int> > V[3];
    int sum = 0;
    for (int i = 0; i < N; ++i) {
        char t; cin >> t;
        auto& which = (t == 'W' ? V[0] : (t == 'L' ? V[1] : V[2]));
        int x, y;
        cin >> x >> y;
        sum += x + y;
        which.emplace_back(x, y);
    }

    vector<int> solve[3];
    for (int i = 0; i < 3; ++i)
        solve[i] = get_best(V[i], sum);

    int answer = 0;
    for (int i = 0; i <= sum; ++i)
        answer = max(answer, solve[0][i] + solve[1][i] + solve[2][i] - i);
    cout << answer << "\n";
}
