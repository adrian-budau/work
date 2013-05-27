#include <iostream>
#include <vector>
#include <array>
#include <string>

using namespace std;

array<string, 3> A = {"R", "P", "S"};

int answer(int64_t N) {
    if (N <= 3)
        return N - 1;
    --N;

    int64_t group = 1;
    while (group * 2 < N) {
        N -= group * 2;
        group *= 3;
    }

    if (group < N)
        return (2 + answer(N - group)) % 3;
    return (1 + answer(N)) % 3;
}

int main() {
    int64_t N;
    while (cin >> N) {
        if (N == 0)
            break;
        cout << A[(answer(N) + 1) % 3] << "\n";
    }
}
