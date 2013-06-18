#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace std;

vector<string> split(string S, const char &character) {
    int last = 0;
    S += '#';

    vector<string> answer;
    for (int i = 0; i < int(S.size()); ++i) {
        if (S[i] == character) {
            answer.push_back(S.substr(last, i - last));
            last = i + 1;
        }
    }
    return answer;
}

class Counter {
  public:
    Counter():
            answer(0) {
    }

    void add(char c) {
        if (++many_[c] > answer)
            answer = many_[c];
    }

    int best() const {
        return answer;
    }

  private:
    int answer;
    map<char, int> many_;
};

int main() {
    int T; cin >> T;

    while (T--) {
        string S; cin >> S;
        vector<string> bits = split(S, '#');

        int N = bits.size();
        vector<int> best(N), best_to(N), best_from(N);

        Counter prefix;
        for (int i = 0; i < N; ++i) {
            Counter now;
            for (string::iterator it = bits[i].begin(); it != bits[i].end(); ++it) {
                prefix.add(*it);
                now.add(*it);
            }

            best[i] = now.best();
            best_to[i] = prefix.best();
        }

        Counter suffix;
        for (int i = N - 1; i >= 0; --i) {
            for (string::iterator it = bits[i].begin(); it != bits[i].end(); ++it)
                suffix.add(*it);
            best_from[i] = suffix.best();
        }

        int answer = 0;
        for (int i = 0; i < N - 3; ++i) {
            int X, Y, Z, T;
            X = best_to[i];
            Y = best[i + 1];
            Z = best[i + 2];
            T = best_from[i + 3];
            if (X > 0 and Y > 0 and Z > 0 and T > 0)
                answer = max(answer, X + Y + Z + T + 3);
        }

        cout << answer << "\n";
    }
}
