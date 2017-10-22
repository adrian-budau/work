#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <inputGenerator.hpp>

using namespace std;
using namespace inputGenerator;

int64_t ways(vector<int> f, int64_t MAXK) {
    int64_t answer = 1;
    for (int total = 0, i = 0; i < int(f.size()); ++i) {
        for (int j = 1; j <= f[i]; ++j) {
            answer = answer * (total + j) / j;
            if (answer > MAXK)
                return MAXK;
        }
        total += f[i];
    }
    return answer;
}

int main(int argc, char **argv) {
    vector<string> arguments(argv + 1, argv + argc);

    if (arguments.size() < 4) {
        cerr << "USAGE: M MAXDIGIT MAXK P\n";
        return 1;
    }

    int M = stoi(arguments[0]);
    int MAXDIGIT = stoi(arguments[1]);
    int64_t MAXK = stoll(arguments[2]);
    double P = stod(arguments[3]);

    Seed::logging = false;
    if (arguments.size() > 4)
        reSeed(stoul(arguments[4]));

    map< vector<int>, int64_t> comb;
    comb[vector<int>()] = 1;
    for (auto &p : comb) {
        auto v = p.first;
        if (v.size() == 10)
            continue;
        auto ways = p.second;

        int total = 0;
        for (auto &x : v)
            total += x;

        int64_t new_ways = ways;
        for (int i = 1; i + total <= MAXDIGIT; ++i) {
            if (v.size() && i > v.back())
                break;
            v.push_back(i);
            new_ways = new_ways * (total + i) / i;
            if (new_ways > MAXK)
                break;
            comb[v] = new_ways;
            v.pop_back();
        }
    }

    vector< pair< vector<int>, int64_t> > combs(comb.begin(), comb.end());

    ios::sync_with_stdio(false);
    cout << M << "\n";
    while (M--) {
        if (randomDouble(0, 1) < P) {
            vector<int> f; int64_t posibilities;
            tie(f, posibilities) = randomElement(combs);
            while (f.size() < 10)
                f.push_back(0);
            randomShuffle(f.begin(), f.end());
            for (int i = 0; i < 10; ++i)
                cout << f[i] << " ";
            cout << randomInt64(1, posibilities) << "\n";
        } else {
            int DIGITS = randomInt(1, MAXDIGIT);
            vector<int> f = randomPartition(DIGITS, 10, 0);
            randomShuffle(f.begin(), f.end());
            int64_t posibilities = ways(f, MAXK);
            for (int i = 0; i < 10; ++i)
                cout << f[i] << " ";
            cout << randomInt64(1, posibilities) << "\n";
        }
    }
}
