#include <iostream>
#include <vector>
#include <string>
#include <inputGenerator.hpp>

using namespace std;
using namespace inputGenerator;

int main(int argv, char **argc) {
    vector<string> arguments(argc + 1, argc + argv);

    if (arguments.size() < 2) {
        cerr << "You need at least 2 arguments N and MAXK\n";
        return -1;
    }

    Seed::logging = false;

    int N = stoi(arguments[0]);
    int MAXK = stoi(arguments[1]);

    if (arguments.size() > 2) {
        reSeed(stoul(arguments[2]));
    }

    int REPEATS = randomInt(1, 3 * ((N - 1) / MAXK + 1));

    string S = randomString(N, letters);

    for (int i = 0; i < REPEATS; ++i) {
        int length = randomInt(1, MAXK);
        int start = randomInt(0, int(S.size()) - length);
        int newStart = randomInt(0, int(S.size()) - length);

        string newS = S.substr(start, length);
        copy(newS.begin(), newS.end(), S.begin() + newStart);
    }

    cout << S << "\n";
}
