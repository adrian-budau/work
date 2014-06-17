#include <iostream>
#include <array>

using namespace std;

int answer(array<long long, 3> remainder, int move) {
    if (remainder[0] < 0 || remainder[1] < 0 || remainder[2] < 0)
        return 4; // undefined
    if (remainder[3 - move] == remainder[move] ||
        remainder[3 - move] == remainder[move] - 1)
        return 2; // draw

    if (remainder[3 - move] > remainder[move])
        remainder[3 - move] = remainder[move];
    if (remainder[move] > remainder[3 - move] + 1)
        remainder[move] = remainder[3 - move] + 1;

    if ((remainder[0] + remainder[1] + remainder[2]) % 2)
        return 3;
    return 1;
}

int main() {
    int T; cin >> T;
    for (int test = 1; test <= T; ++test) {
        cout << "Case " << test << ": ";

        int N; cin >> N;

        array<long long, 3> remainder{0, 0, 0};

        for (int i = 0; i < N; ++i) {
            int L, B; cin >> L >> B;
            remainder[B % 3] += 1LL * (L / 2) * (B - 1);
            remainder[(1LL * B * B) % 3] += 1LL * (L - 1 - L / 2) * (B - 1);
        }

        if (remainder[1] == 0 && remainder[2] == 0) {
            if (remainder[0] == 0) {
                cout << "Draw\n";
                continue;
            }

            cout << "J\n";
            continue;
        }

        --remainder[1];
        int outcome1 = answer(remainder, 1);
        ++remainder[1];
        --remainder[2];
        int outcome2 = answer(remainder, 2);
        ++remainder[2];

        int answer;
        if (outcome1 == 1 || outcome2 == 1)
            answer = 1;
        else if (outcome1 == 2 || outcome2 == 2)
            answer = 2;
        else
            answer = 3;
        if (answer == 1)
            cout << "M";
        else if (answer == 2)
            cout << "Draw";
        else
            cout << "J";
        cout << "\n";
    }
}
