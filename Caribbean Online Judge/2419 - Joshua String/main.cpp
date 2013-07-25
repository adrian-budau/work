#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <algorithm>
#include <vector>

using namespace std;

class compare {
  public:
    bool operator()(const string &A, const string &B) const {
        return A + B < B + A;
    }
};

int main() {
    int T; cin >> T;

    while (T--) {
        int N; cin >> N;

        multiset<string, compare> S;
        for (int i = 0; i < N; ++i) {
            string V; cin >> V;
            S.insert(V);
        }

        string answer;
        while (S.size()) {
            string V = *S.begin();
            S.erase(S.begin());
            answer += V[0];
            if (V.size() > 1)
                S.insert(V.substr(1));
        }

        cout << answer << "\n";
    }
}
