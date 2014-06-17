#include <fstream>
#include <string>
#include <vector>

#include <inputGenerator.hpp>

using namespace std;
using namespace inputGenerator;

int main(int argv, char **argc) {
    vector<string> arguments(argc + 1, argc + argv);

    Seed::logging = false;

    if (arguments.size() < 3) {
        cout << "At least 3 parameters are required, N M and K\n";
        return -1;
    }

    int N = stoi(arguments[0]);
    int M = stoi(arguments[1]);
    int K = stoi(arguments[2]);

    if (arguments.size() >= 4)
        reSeed(stoi(arguments[3]));

    cout << N << " " << M << " " << K << "\n";

    for (int i = 0; i < M; ++i) {
        int odds = N / 2;
        int evens = N - N / 2;
        int64_t odd_total = int64_t(odds + 1) * odds / 2;
        int64_t even_total = int64_t(evens + 1) * evens / 2;

        int first, second;
        if (randomInt64(0, odd_total + even_total) < odd_total) {
            auto sample = randomSample(2, 0, odds);
            first = sample[0];
            second = sample[1] - 1;

            first = 2 * first + 1;
            second = 2 * second + 1;
        } else {
            auto sample = randomSample(2, 0, evens);
            first = sample[0];
            second = sample[1] - 1;

            first = 2 * first;
            second = 2 * second;
        }

        first += 1;
        second += 1;

        cout << first << " " << second << "\n";
    }

    for (int i = 0; i < K; ++i) {
        auto sample = randomSample(2, 0, N);
        int first = sample[0] + 1;
        int second = sample[1];
        cout << first << " " << second << "\n";
    }
}
