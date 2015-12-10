#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

using namespace std;

int main() {
    ifstream cin("hercule.in");
    ofstream cout("hercule.out");

    int T; cin >> T;
    while (T--) {
        int N; cin >> N;
        if (N == 3 || N == 5 || N == 6 || N == 9 || N == 10) {
            cout << "-1\n";
            continue;
        }
        if (N == 1) {
            cout << "0\n";
            continue;
        }

        vector<int> answer{1};
        N -= 2;
        int sum = 2;
        while (N > 0) {
            if (sum > N) {
                answer.push_back(N);
                break;
            } else {
                answer.push_back(sum);
                N -= sum;
                sum *= 2;
            }
        }

        if (find(answer.begin(), answer.end() - 1, answer.back()) != answer.end() - 1) {
            if (answer.back() == 1) {
                ++answer.back();
                --answer[answer.size() - 2];
            }

            ++answer.back();
            --answer[answer.size() - 2];
        }
        cout << answer.size() << "\n";
        for (auto &x : answer)
            cout << x << " ";
        cout << "\n\n";
    }
}

