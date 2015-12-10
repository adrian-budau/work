#include <vector>
#include <algorithm>
#include <fstream>
#include <inputGenerator.hpp>
#include <string>

using namespace std;
using namespace inputGenerator;

int main(int argc, char* argv[]) {
    vector<string> arguments(argv, argv + argc);

    if (arguments.size() < 4) {
        cerr << "Generator error: Expecting 3 arguments N Q MAXV\n";
        return 1;
    }

    int N = stoi(arguments[1]), Q = stoi(arguments[2]), MAXV = stoi(arguments[3]);
    cout << Q << "\n";
    Seed::logging = false;
    if (arguments.size() > 4)
        reSeed(stoul(arguments[4]));

    auto points = randomSample(N, 0, MAXV * MAXV - 1);
    vector<pair<int, int>> realPoints;
    for (int i = 0; i < N; ++i)
        realPoints.emplace_back(points[i] / MAXV + 1, points[i] % MAXV + 1);

    randomShuffle(realPoints.begin(), realPoints.end());

    int deletes = randomInt(0, min(N / 2, max(Q - N - 1, 0)));

    vector<int> ops(Q);
    for (int i = 0; i < N; ++i)
        ops[i] = 1;
    for (int i = N; i < N + deletes; ++i)
        ops[i] = 2;
    for (int i = N + deletes; i < Q; ++i)
        ops[i] = 3;

    randomShuffle(ops.begin() + N / 2 + 1, ops.end() - 1);

    vector< pair<int, int> > now;
    for (auto &op: ops) {
        cout << op << " ";
        if (op == 1) {
            now.push_back(realPoints.back());
            cout << realPoints.back().first << " " << realPoints.back().second << "\n";
            realPoints.pop_back();
        } else if (op == 2) {
            auto pos = randomUInt(0, now.size() - 1);
            cout << now[pos].first << " " << now[pos].second << "\n";
            swap(now[pos], now.back());
            now.pop_back();
        } else {
            int X = randomInt<int>(0, MAXV / 2 * now.size());
            int Y = randomInt<int>(0, MAXV / 2 * now.size());
            cout << X << " " << Y << "\n";
        }
    }
}
