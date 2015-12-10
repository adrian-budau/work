#include <iostream>
#include <vector>
#include <algorithm>
#include <inputGenerator.hpp>
#include <cassert>
using namespace std;
using namespace inputGenerator;

int main(int argv, char **argc) {
    vector<string> arguments(argc + 1, argc + argv);

    if (arguments.size() < 3) {
        cerr << "Need at least 3 arguments N (minimum number of elements), M (number of operations) and K(different values)\n";
        return -1;
    }

    int N = stoi(arguments[0]);
    int M = stoi(arguments[1]);
    int K = stoi(arguments[2]);
    Seed::logging = false;
    if (arguments.size() > 3)
        reSeed(stoul(arguments[3]));

    vector<int> op(M);
    for (int i = 0; i < N; ++i)
        op[i] = 1;

    // we do it in charges, pop left and push right and push left and pop right
    int direction = randomElement({-1, 1});
    int left = randomInt(1, int(sqrt(N)));
    int size = N;
    for (int i = N; i < M; ++i) {
        if (randomElement({true, false})) {
            op[i] = 5;
            continue;
        }

        op[i] = 1;
        if (direction == -1)
            op[i] = 2;
        if (size > N && randomElement({true, false})) {
            op[i] = 5 - op[i];
            --size;
        } else
            ++size;
        if (--left == 0) {
            left = randomInt(1, int(sqrt(N)));
            direction = randomElement({-1, 1});
        }
    }

    cout << M << "\n";
    vector<int> values = randomSample(K, 1, M);
    size = 0;
    for (int i = 0; i < M; ++i) {
        assert(1 <= op[i] && op[i] <= 5);
        cout << op[i] << " ";
        if (op[i] == 5) {
            int x = randomInt(1, size);
            int y = randomInt(1, size);
            if (x > y)
                swap(x, y);
            int k = randomInt(1, y - x + 1);
            cout << x << " " << y << " " << k << "\n";
            continue;
        }

        if (op[i] <= 2) {
            cout << randomElement(values) << "\n";
            ++size;
        } else {
            --size;
            cout << "\n";
        }
    }
}
