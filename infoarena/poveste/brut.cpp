#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

vector<string> transpose(vector<string> S) {
    int N = S.size();
    int M = S[0].size();
    vector<string> answer(M, string(N, ' '));
    for (int i = 0; i < M; ++i)
        for (int j = 0; j < N; ++j)
            answer[i][j] = S[j][i];
    return answer;
}

int main() {
    ifstream cin("poveste.in");
    ofstream cout("poveste.out");

    int T; cin >> T;
    while (T--) {
        int N, M; cin >> N >> M;
        vector<string> S(N);
        for (int i = 0; i < N; ++i)
            cin >> S[i];
        S = transpose(S);

        int answer = N;
        swap(N, M);
        for (int i = 0; i < N; ++i)
            for (int j = i + 1; j < N; ++j) {
                int longest = 0;
                int aux = 0;
                for (int k = 0; k < M; ++k)
                    if (S[i][k] == S[j][k])
                        longest = max(longest, ++aux);
                    else
                        aux = 0;
                answer = max(answer, longest * (j - i + 1));
            }
        cout << answer << "\n";
    }
}
