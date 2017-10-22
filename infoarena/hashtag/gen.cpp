#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <inputGenerator.hpp>

using namespace std;
using namespace inputGenerator;

int main(int argc, char **argv) {
    vector<string> arguments(argv + 1, argv + argc);

    if (arguments.size() < 5) {
        cout << "Expecint 5 arguments: N M K P1 P2\n";
    }

    if (arguments.size() > 5)
        reSeed(stoul(arguments[5]));

    int N = stoi(arguments[0]);
    int M = stoi(arguments[1]);
    int K = stoi(arguments[2]);
    double P1 = stod(arguments[3]);
    double P2 = stod(arguments[4]);

    vector<string> S(N, string(M, '.'));

    int col1 = randomInt(1, K);
    int col2 = randomInt(1, K);
    int line1 = randomInt(1, K);
    int line2 = randomInt(1, K);

    auto lineParts = randomPartition(N - line1 - line2 - 3, 3, 0);
    lineParts[0]++; lineParts[1]++; lineParts[2]++;
    auto colParts = randomPartition(N - col1 - col2 - 3, 3, 0);
    colParts[0]++; colParts[1]++; colParts[2]++;
    int line[] = {lineParts[0], lineParts[0] + line1, lineParts[0] + line1 + lineParts[1], lineParts[0] + line1 + lineParts[1] + line2};
    int col[] = {colParts[0], colParts[0] + col1, colParts[0] + col1 + colParts[1], colParts[0] + col1 + colParts[1] + col2};

    for (int i = 0; i < 4; i += 2)
        for (int j = line[i]; j < line[i + 1]; ++j)
            for (int k = 0; k < M; ++k)
                S[j][k] = '#';
    for (int i = 0; i < 4; i += 2)
        for (int j = col[i]; j < col[i + 1]; ++j)
            for (int k = 0; k < N; ++k)
                S[k][j] = '#';

    auto flip = [&](int x, int y) {
        if (S[x][y] == '#')
            S[x][y] = '.';
        else
            S[x][y] = '#';
    };

    vector< pair<int, int> > in, out;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j)
            if (S[i][j] == '#')
                in.emplace_back(i, j);
            else
                out.emplace_back(i, j);

    for (auto &p : randomSubsequence(in, P1 * in.size()))
        flip(p.first, p.second);

    for (auto &p : randomSubsequence(out, P2 * out.size()))
        flip(p.first, p.second);

    cout << N << " " << M << "\n";
    for (auto &s : S)
        cout << s << "\n";
}
