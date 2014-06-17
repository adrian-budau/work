#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    int T; cin >> T;
    for (int test = 1; test <= T; ++test) {
        int N; cin >> N;
        vector< vector<int> > groups;
        string real;

        bool bad = false;
        for (int i = 0; i < N; ++i) {
            string S; cin >> S;

            string now = "";
            vector<int> current;
            int many = 0;
            for (int j = 0; j < int(S.size()); ++j) {
                ++many;
                if (j == int(S.size()) - 1 || S[j] != S[j + 1]) {
                    now += S[j];
                    current.push_back(many);
                    many = 0;
                }
            }

            if (real == "")
                real = now;
            else if (real != now)
                bad = true;

            groups.push_back(current);
        }

        cout << "Case #" << test << ": ";
        if (bad)
            cout << "Fegla Won\n";
        else {
            int answer = 0;
            for (int cols = 0; cols < int(groups[0].size()); ++cols) {
                vector<int> column;
                for (int i = 0; i < int(groups.size()); ++i)
                    column.push_back(groups[i][cols]);

                sort(column.begin(), column.end());

                int destination = column[column.size() / 2];
                for (auto v : column)
                    answer += max(v - destination, destination - v);
            }
            cout << answer << "\n";
        }
    }
}
