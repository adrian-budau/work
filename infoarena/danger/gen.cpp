#include <iostream>
#include <fstream>
#include <inputGenerator.hpp>

using namespace std;
using namespace inputGenerator;

int main(int argc, char **argv) {
    vector<string> arguments(argv + 1, argv + argc);

    if (arguments.size() < 5) {
        cout << "USAGE: N M MINV MAXV MAXR\n";
        return 1;
    }

    int N = stoi(arguments[0]);
    int M = stoi(arguments[1]);
    int MINV = stoi(arguments[2]);
    int MAXV = stoi(arguments[3]);
    int MAXR = stoi(arguments[4]);
    Seed::logging = false;
    cout << N << " " << M << "\n";
    for (int i = 0; i < N; ++i) {
        int mid = randomInt(MINV, MAXV);
        int start = max(MINV, mid - randomInt(0, MAXR));
        int end = min(MAXV, mid + randomInt(0, MAXR));
        for (int j = 0; j < M; ++j)
            cout << randomInt(start, end) << " ";
        cout << "\n";
    }
}
