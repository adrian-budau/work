#include <iostream>
#include <fstream>
#include <bitset>
#include <inputGenerator.hpp>

using namespace std;
using namespace inputGenerator;

int main(int argc, char **argv) {
    vector<string> arguments(argv + 1, argv + argc);

    if (arguments.size() < 2) {
        cerr << "Expecting 2 arguments: N and prob\n";
        return -1;
    }

    Seed::logging = false;
    if (arguments.size() > 2) {
        reSeed(stoul(arguments[2]));
    }

    int N = stoi(arguments[0]);
    double prob = stod(arguments[1]);
    bool picked = false;
    string pattern;
    for (int i = 1; N; ++i) {
        string S = bitset<32>(i).to_string();
        S = S.substr(find(S.begin(), S.end(), '1') - S.begin());
        for (auto &c : S) {
            if (N == 0)
                break;
            if (picked) {
                if (pattern.back() == c) {
                    picked = false;
                    --N;
                }
                continue;
            }
            if (randomDouble(0, 1) < prob) {
                pattern += '1' ^ '0' ^ c;
                picked = true;
            } else {
                char v = randomElement({'1', '0'});
                pattern += v;
                if (c != v)
                    picked = true;
                else
                    --N;
            }
        }
    }

    cout << pattern << "\n";
}
