#include <iostream>
#include <inputGenerator.hpp>
#include <fstream>
#include <vector>

using namespace std;
using namespace inputGenerator;

int main(int argv, char **argc) {
    Seed::logging = false;
    vector<string> arguments(argc + 1, argc + argv);

    // N M MAXVALUES PROBABILITY SMALL_VALUES
    if (arguments.size() < 5) {
        cout << "Too few arguments, at least 5 required";
        return -1;
    }

    if (arguments.size() > 5)
        reSeed(stoi(arguments[5]));

    int N = stoi(arguments[0]);
    int M = stoi(arguments[1]);
    int MAXVALUES = stoi(arguments[2]);
    double PROBABILITY = stod(arguments[3]);
    int SMALL_VALUES = stoi(arguments[4]);

    vector<int> V(N);
    for (int i = 0; i < N; ++i)
        if (randomDouble(0, 1) <= PROBABILITY)
            V[i] = randomInt(1, max<int>(1, int64_t(MAXVALUES) * (N - i) / N));
        else
            V[i] = randomInt(1, SMALL_VALUES);

    cout << N << "\n";
    for (int i = 0; i < N; ++i)
        cout << V[i] << " ";
    cout << "\n";
    cout << M << "\n";
    for (int i = 0; i < M; ++i) {
        if (randomElement({true, true, true, true, true, false})) {
            int position;
            do {
                position = randomInt(0, N - 1);
                int64_t sum = V[position];
                int steps = 0;
                int now = position - 1;
                while (now >= 0 and V[now] < sum) {
                    sum += V[now--];
                    ++steps;
                    if (steps > 10)
                        break;
                }
                if (steps > 10 || N <= 100)
                    break;
            } while (true);
            cout << 1 << " " << position + 1 << "\n";
        } else {
            cout << 0 << " ";
            int position = randomInt(1, N), new_value;
            cout << position << " ";
            if (randomDouble(0, 1) <= PROBABILITY)
                new_value = randomInt(1, max<int>(1, int64_t(MAXVALUES) * (N - position) / N));
            else
                new_value = randomInt(1, SMALL_VALUES);
            V[position - 1] = new_value;
            cout << new_value;
            cout << "\n";
        }
    }
}
