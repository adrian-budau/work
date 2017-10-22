#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

int main() {
    ifstream cin("magnet.in");
    ofstream cout("magnet.out");

    int T; assert(cin >> T);
    assert(1 <= T && T <= 10);

    while (T--) {
        int N; assert(cin >> N);
        assert(1 <= N && N <= 10000);
        vector<int> A(N);
        for (int i = 0; i < N; ++i) {
            assert(cin >> A[i]);
            assert(0 <= A[i] && A[i] <= 1000 * 1000 * 1000);
        }
        vector<int> B(N);
        for (int j = 0; j < N; ++j) {
            assert(cin >> B[j]);
            assert(0 <= B[j] && B[j] <= 1000 * 1000 * 1000);
        }

        sort(B.begin(), B.end());
        sort(A.begin(), A.end());

        vector<int> sub;
        vector<int> nextA(1, A[0]), nextB(1, B[0]);
        for (int i = 1; i < N; ++i) {
            int diffA = A[i] - A[i - 1];
            int diffB = B[i] - B[i - 1];

            if (diffA == 0 && diffB == 0)
                continue;
            nextA.push_back(A[i]);
            nextB.push_back(B[i]);
            sub.emplace_back(diffA - diffB);
        }

        if (*min_element(sub.begin(), sub.end()) < 0) {
            cout << "-1\n";
            continue;
        }
        A = nextA;
        B = nextB;
        N = A.size();
        // special case from right to left for the first part, rest is easy
        int first0 = find(sub.begin(), sub.end(), 0) - sub.begin();
        bool sol = true;
        vector<bool> pressed(N, false);
        if (first0 != 0) {
            for (int i = first0 - 1; i >= 0; --i)
                if (sub[i] % 2 == 1) { // it's odd, we press right before
                    pressed[i] = true;
                    --sub[i];
                    if (i > 0)
                        --sub[i - 1];
                }
        }
        // now the rest form left to right
        for (int i = first0; i < N - 1; ++i) {
            if (sub[i] == 0)
                continue;
            int j;
            for (j = i; j < N - 1 && sub[j] > 0; ++j);
            // so from i to j - 1 we have > 0, wee, that means we can press from i + 1 to j - 1
            for (int k = i; k + 1 < j; ++k)
                if (sub[k] % 2) {
                    --sub[k];
                    --sub[k + 1];
                    pressed[k + 1] = true;
                }
            i = j;
        }

        // we might have one last part odd, we can fix it easily
        if (sub[N - 2] % 2) {
            pressed[N - 1] = true;
            --sub[N - 2];
        }

        if (sub[0] % 2) {
            pressed[0] = true;
            --sub[0];
        }

        // we might have no solution if we still have odds
        for (int i = 0; i < N - 1; ++i)
            if (sub[i] % 2)
                sol = false;
        if (!sol) {
            cout << "-1\n";
            continue;
        }

        // now just simulate
        int pulled_left = 0, first = A[0];
        vector< pair<int, int> > answer;
        for (int i = 0; i < N; ++i) {
            int now = A[i] - pulled_left;
            if (pressed[i]) {
                answer.emplace_back(A[i] - pulled_left, 1);
                pulled_left++;
                if (i > 0)
                    ++first;
            }
            if (i < N - 1) {
                // put right in between
                // we have to put right in between, taking into account we might press the other one
                int mid = (now + A[i + 1] - pulled_left) / 2;
                answer.emplace_back(mid, sub[i] / 2);
                first += sub[i] / 2;
                pulled_left += sub[i] / 2;
            }
        }

        // pull them all back to the rightful position
        if (first > B[0])
            answer.emplace_back(B[0], first - B[0]);
        else
            answer.emplace_back(B[N - 1], B[0] - first);
        cout << answer.size() << "\n";
        for (auto &p : answer)
            cout << p.first << " " << p.second << "\n";
    }
}
