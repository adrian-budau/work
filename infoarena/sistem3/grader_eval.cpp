#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>


using namespace std;

void exit(string S, int points) {
    cerr << S;
    cout << points;
    exit(0);
}

int main() {
    ifstream input("sistem3.in");
    ifstream output("sistem3.out");

    int N, kModulo; input >> N >> kModulo;

    vector< vector< pair<int, int> > > edges(N);

    vector<int> gradient_(N);

    for (int i = 0; i < N; ++i) {
        int x, y, w; input >> x >> y >> w;
        --x; --y;
        edges[x].emplace_back(y, w);
        edges[y].emplace_back(x, w);
    }

    for (int i = 0; i < N; ++i)
        input >> gradient_[i];

    if (!output)
        exit("Fisier de iesire lipsa", 0);

    vector<int> potential_(N);
    for (int i = 0; i < N; ++i)
        if (!(output >> potential_[i]))
            exit("Fisier de iesire incomplet", 0);

    for (int i = 0; i < N; ++i)
        if (potential_[i] < 0 || potential_[i] >= kModulo)
            exit("Wrong answer", 0);

    for (int i = 0; i < N; ++i) {
        int current_gradient = 0;
        for (auto &next : edges[i]) {
            int aux = (potential_[next.first] - potential_[i]) * next.second % kModulo;
            current_gradient = (current_gradient + kModulo + aux) % kModulo;
        }

        if (current_gradient != gradient_[i])
            exit("Wrong answer", 0);
    }

    exit("OK", 10);
}
