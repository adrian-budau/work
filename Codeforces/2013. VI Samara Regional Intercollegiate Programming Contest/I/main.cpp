#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int kInfinite = 2000000000;

int main() {
    int N; cin >> N;

    if (N == 1) {
        cout << "1\n";
        return 0;
    }
    vector<int> B(N + 1);
    for (int i = 1; i <= N; ++i)
        cin >> B[i];

    vector<long long> answer;
    for (int i = N - 1; i > 0; --i) {
        if (B[i] == 1) {
            if (i == N - 1) {
                answer.push_back(1);
                continue;
            }

            if (answer.front() < 0) {
                vector<long long> new_answer(1, 1);
                reverse(answer.begin(), answer.end());
                for (auto &x: answer)
                    new_answer.push_back(new_answer.back() - x);
                reverse(new_answer.begin(), new_answer.end());
                swap(answer, new_answer);
            } else {
                vector<long long> new_answer(1, 1);
                for (auto &x: answer)
                    new_answer.push_back(new_answer.back() + x);
                swap(answer, new_answer);
            }
        } else {
            if (i == N - 1) {
                answer.push_back(-1LL);
                continue;
            }

            if (answer.front() < 0)  {
                vector<long long> new_answer(1, -1);
                for (auto &x: answer)
                    new_answer.push_back(new_answer.back() + x);
                swap(answer, new_answer);
            } else {
                vector<long long> new_answer(1, -1);
                reverse(answer.begin(), answer.end());
                for (auto &x: answer)
                    new_answer.push_back(new_answer.back() - x);
                reverse(new_answer.begin(), new_answer.end());
                swap(answer, new_answer);
            }
        }

        if (*max_element(answer.begin(), answer.end()) > kInfinite || *min_element(answer.begin(), answer.end()) < -kInfinite) {
            cout << "IMPOSSIBLE\n";
            return 0;
        }
    }

    vector<long long> new_answer;
    if (answer.front() < 0)
        new_answer.push_back(kInfinite / 2);
    else
        new_answer.push_back(-kInfinite / 2);

    for (auto &x: answer)
        new_answer.push_back(new_answer.back() + x);

    swap(answer, new_answer);

    if (*max_element(answer.begin(), answer.end()) > kInfinite / 2 || *min_element(answer.begin(), answer.end()) < -kInfinite / 2) {
        cout << "IMPOSSIBLE\n";
        return 0;
    }

    for (auto &x: answer)
        cout << x << " ";
    cout << "\n";
}
