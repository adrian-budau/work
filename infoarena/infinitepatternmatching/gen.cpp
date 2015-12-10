#include <iostream>
#include <vector>
#include <inputGenerator.hpp>

using namespace std;
using namespace inputGenerator;

static const string from0 = "01101110010111011110001001101010111100110111101111100001";

int64_t fromString(const string& S) {
    int64_t v = 0;
    for (auto &c : S)
        v = v * 2 + c - '0';

    return v;
}

string toString(int64_t number) {
    string S;
    while (number) {
        S += number % 2 + '0';
        number /= 2;
    }
    reverse(S.begin(), S.end());
    return S;
}

int main(int argc, char **argv) {
    vector<string> arguments(argv + 1, argv + argc);

    if (arguments.size() < 2) {
        cerr << "Expecting 2 arguments: N parts\n";
        return -1;
    }

    Seed::logging = false;
    int N = stoi(arguments[0]);
    int parts = stoi(arguments[1]);

    if (arguments.size() > 2) {
        reSeed(stoul(arguments[2]));
    }

    if (parts <= 0) { // special case for when we take the beginning
        cout << from0.substr(0, N) << "\n";
        return 0;
    }

    int64_t number = randomInt(1LL << (parts - 1), (1LL << parts) - 1);
    string aux;
    while (int(aux.size()) <= N)
        aux += toString(number++);

    cout << aux.substr(randomUInt(0, aux.size() - N), N) << "\n";
}
