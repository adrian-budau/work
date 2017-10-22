#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <set>
#include <inputGenerator.hpp>

using namespace std;
using namespace inputGenerator;

vector<string> randomColours(int many) {
    vector<string> base;
    set<string> have;
    ifstream colours("colours.txt");
    string S;
    while (getline(colours, S)) {
        string without_whitespace;
        for (auto &c : S)
            if (c != ' ')
                without_whitespace += c;
        assert(have.count(without_whitespace) == 0);
        base.push_back(without_whitespace);
        have.emplace(without_whitespace);
    }

    for (int i = have.size(); i < many; ++i) {
        while (true) {
            string word = randomString(randomInt(1, 20), lowerLetters + upperLetters);
            if (have.count(word))
                continue;
            have.emplace(word);
            base.emplace_back(word);
            break;
        }
    }
    return shuffle(base);
}

int main(int argc, char **argv) {
    vector<string> arguments(argv + 1, argv + argc);
    if (arguments.size() < 6) {
        cerr << "USAGE: N K X Y PART MAXV\n";
        return 1;
    }

    int N = stoi(arguments[0]);
    int K = stoi(arguments[1]);
    int X = stoi(arguments[2]);
    int Y = stoi(arguments[3]);
    int PART = stoi(arguments[4]);
    int MAXV = stoi(arguments[5]);
    Seed::logging = false;
    if (arguments.size() > 6)
        reSeed(stoul(arguments[6]));

    assert(1 <= K && K <= N);
    assert(1 <= PART);
    assert(PART <= X);
    assert(PART <= Y);
    assert(PART <= N);
    assert(max(X, Y) <= N);
    ios::sync_with_stdio(false);

    vector<int> Xs = randomPartition(X, PART, 1);
    vector<int> Ys = randomPartition(Y, PART, 1);
    vector<int> Ns(PART, 0);
    for (int i = 0; i < PART; ++i) {
        Ns[i] = max(Xs[i], Ys[i]);
        N -= Ns[i];
    }
    if (N < 0)
        N = 0;
    vector<int> Nextra = randomPartition(N, PART, 0);
    for (int i = 0; i < PART; ++i)
        Ns[i] += Nextra[i];

    N = 0;
    for (auto &n : Ns)
        N += n;
    for (int i = 0; i < PART; ++i)
        if (randomElement({true, false}))
            swap(Xs[i], Ys[i]);
    vector< pair<int, int> > socks;
    int deltax = 0, deltay = 0;
    for (int i = 0; i < PART; ++i) {
        vector<int> left, right;
        for (int j = 0; j < Xs[i]; ++j)
            left.emplace_back(deltax + j);
        for (int j = 0; j < Ys[i]; ++j)
            right.emplace_back(deltay + j);
        for (int j = left.size(); j < Ns[i]; ++j)
            left.emplace_back(randomInt(deltax, deltax + Xs[i] - 1));
        for (int j = right.size(); j < Ns[i]; ++j)
            right.emplace_back(randomInt(deltay, deltay + Ys[i] - 1));
        randomShuffle(left.begin(), left.end());
        randomShuffle(right.begin(), right.end());
        for (int j = 0; j < Ns[i]; ++j)
            socks.emplace_back(left[j], right[j]);
        deltax += Xs[i];
        deltay += Ys[i];
    }

    assert(deltay <= MAXV);

    randomShuffle(socks.begin(), socks.end());
    vector<string> colours = randomColours(deltax);
    vector<int> numbers = shuffle(randomSample(deltay, 1, MAXV));
    set< pair<int, int> > set_socks(socks.begin(), socks.end());
    N = set_socks.size();
    K = min(K, N);
    cout << set_socks.size() << " " << K << "\n";
    socks = vector< pair<int, int> >(set_socks.begin(), set_socks.end());
    for (auto &p : shuffle(socks))
        cout << colours[p.first] << " " << numbers[p.second] << "\n";
}

