#include <iostream>
#include <fstream>
#include <set>
#include <vector>

using namespace std;

int main() {
    int N, A, B; cin >> N >> A >> B;
    set<int> time;
    for (int i = 0; i < N; ++i) {
        int x; cin >> x;
        time.emplace(x);
    }

    vector<int> answer;
    set<int> unanswered;
    for (int t = 0; t <= 1000 + B; ++t) {
        if (time.count(t)) {
            time.erase(t);
            unanswered.emplace(t);
        }
        if (int(unanswered.size()) == A || (unanswered.size() && *unanswered.begin() + B == t)) {
            int answered = (unanswered.size() + 1) / 2;
            for (int i = 0; i < answered; ++i) {
                answer.push_back(t);
                unanswered.erase(unanswered.begin());
            }
        }
    }
    for (auto &x : answer)
        cout << x << " ";
    cout << "\n";
}
