#include <iostream>
#include <fstream>
#include <string>

using namespace std;

inline bool good(int mask, int M, int N) {
    int first = 0;
    int second = 0;
    for (int i = 0; i < M; ++i) {
        if ((1 << i) & mask)
            ++first;
        else
            ++second;
    }

    if (first == N and second < N and ((1 << (M - 1) & mask)))
        return true;
    if (first < N and second == N and ((1 << (M - 1) & mask) == 0))
        return true;

    return false;
}

int main() {
    ifstream cin("input.txt");
    ofstream cout("output.txt");

    int N, M; cin >> N >> M;

    string S; cin >> S;
    
    int mask = 0;
    for (int i = 0; i < M; ++i)
        if (S[i] == '1')
            mask += (1 << i);

    int answer = 0;
    for (int i = 0; i < M; ++i) {
        if (i == 0) {
            ++answer;
            continue;
        }

        int current = 0;
        for (int j = 0; j < (1 << (M - i)); ++j) {
            int first = (mask & ((1 << i) - 1)) + (j << i);

            if (good(first, M, N)) {
                current |= (1 << (j % 2));
            }

            if (current == 3)
                break;
        }

        if (current == 3)
            ++answer;
    }

    cout << answer << "\n";
}
