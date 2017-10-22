#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <inputGenerator.hpp>

using namespace std;
using namespace inputGenerator;

int main(int argc, char **argv) {
    vector<string> arguments(argv + 1, argv + argc);

    if (arguments.size() < 4) {
        cerr << "Usage: N M K answer\n";
        return 1;
    }

    Seed::logging = false;
    int N = stoi(arguments[0]);
    int M = stoi(arguments[1]);
    int K = stoi(arguments[2]);
    int answer = stoi(arguments[3]);

    auto enemyPokemons = randomSample(K, 0, N - 1);

    vector< array<int, 3> > enemy;
    for (int i = 0; i < M; ++i)
        enemy.push_back({{randomElement(enemyPokemons), randomElement(enemyPokemons), randomElement(enemyPokemons)}});

    vector< vector<int> > beats(N, vector<int>(N, 0));
    for (int i = 0; i < N; ++i)
        if (count(enemyPokemons.begin(), enemyPokemons.end(), i)) {
            bool found = false;
            for (int j = 0; j < N; ++j)
                if (i != j && !beats[i][j] && randomDouble(0, 1) < 1.0 / answer) {
                    found = true;
                    beats[j][i] = 1;
                }
            if (!found && answer <= N * N) {
                vector<int> possible;
                for (int j = 0; j < N; ++j)
                    if (i != j && !beats[i][j])
                        possible.push_back(j);
                beats[randomElement(possible)][i] = 1;
            }
        }
    for (int i = 0; i < N; ++i)
        if (count(enemyPokemons.begin(), enemyPokemons.end(), i) == 0) {
            for (int j = 0; j < N; ++j)
                if (i != j && !beats[i][j] && randomElement({true, false}))
                    beats[j][i] = 1;
        }
    cout << N << " " << M << "\n";
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j)
            cout << beats[i][j] << " ";
        cout << "\n";
    }
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < 3; ++j)
            cout << enemy[i][j] + 1 << " ";
        cout << "\n";
    }
}
