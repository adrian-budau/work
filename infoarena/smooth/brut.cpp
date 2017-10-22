#include <iostream>
#include <fstream>

using namespace std;

int f(int64_t x) {
    int twos = 0, threes = 0;
    while (x % 2 == 0) {
        x /= 2;
        ++twos;
    }
    while (x % 3 == 0) {
        x /= 3;
        ++threes;
    }

    for (int i = 5; i <= 7; i += 2)
        while (x % i == 0)
            x /= i;
    if (x > 1)
        return 0;
    // only 2 and 3 matter, at most we can use
    int most = twos + threes;
    // at the least we have to fix some 6
    int least = twos + threes;
    for (int six = 0; six <= twos && six <= threes && six < 6; ++six) {
        int aux = six + (twos - six) / 3 + ((twos - six) % 3 > 0)
            + (threes - six) / 2 + ((threes - six) % 2 > 0);
        least = min(least, aux);
    }
    return most - least + 1;
}

int main() {
    int64_t A, B; cin >> A >> B;
    int64_t answer = 0;
    for (auto i = A; i <= B; ++i)
        answer += f(i);
    cout << answer << "\n";
}
