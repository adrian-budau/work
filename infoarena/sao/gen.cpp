#include <iostream>
#include <fstream>
#include <inputGenerator.hpp>
#include <queue>

using namespace std;
using namespace inputGenerator;

int main(int argc, char **argv) {
    vector<string> arguments(argv + 1, argv + argc);

    if (arguments.size() < 6) {
        cerr << "USAGE: N M K NEGATIVES MAXHP LIMIT";
        return -1;
    }

    int N = stoi(arguments[0]);
    int M = stoi(arguments[1]);
    int K = stoi(arguments[2]);
    int NEGATIVES = stoi(arguments[3]);
    int MAXHP = stoi(arguments[4]);
    int LIMIT = stoi(arguments[5]);

    Seed::logging = false;
    if (arguments.size() > 6)
        reSeed(stoul(arguments[6]));

    vector<int> hp(N);

    for (int i = 0; i < N; ++i) {
        hp[i] = randomInt(1, MAXHP);
        if (i < NEGATIVES)
            hp[i] = -hp[i];
    }

    hp = shuffle(hp);
    int64_t origK = K, origM = M;

    int i;
    priority_queue<int> P;
    for (i = 0; i < N; ++i) {
        K -= hp[i];
        P.push(hp[i]);
        if (K <= 0) {
            if (i >= LIMIT)
                break;
            if (M == 0) {
                if (K == 0)
                    ++i;
                break;
            }
            K += P.top();
            P.pop();
            --M;
        }
    }

    K = origK; M = origM - M;

    cout << N << " " << M << " " << K << "\n";
    for (auto &x : hp)
        cout << x << " ";
    cout << "\n";
}

