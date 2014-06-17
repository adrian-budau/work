#include <iostream>
#include <string>

#include <inputGenerator.hpp> // https://github.com/adrian-budau/inputGenerator

using namespace std;
using namespace inputGenerator;

int is_prime(int number) {
    if (number < 2)
        return false;
    for (int i = 2; i * i <= number; ++i)
        if (number % i == 0)
            return false;
    return true;
}

int main(int argv, char **argc) {
    Seed::logging = false;

    vector<string> arguments(argc + 1, argc + argv);

    /**
     * arguments: N CYCLE_LENGTH MODULO 
     */

    if (arguments.size() < 3) {
        cout << "3 Arguments expected at least" << endl;
        return -1;
    }

    int N, CYCLE_LENGTH, MODULO;
    N = stoi(arguments[0]);
    CYCLE_LENGTH = stoi(arguments[1]);
    MODULO = stoi(arguments[2]);

    if (CYCLE_LENGTH > N) {
        cout << "CYCLE_LENGTH should be smaller than N" << endl;
        return -1;
    }

    while (not is_prime(MODULO))
        ++MODULO;

    if (arguments.size() > 3) {
        reSeed(stoul(arguments[3]));
    }

    auto G = chain<int, int>(CYCLE_LENGTH, Boolean::False);
    addEdge(G[0], G[G.size() - 1]);

    for (int i = CYCLE_LENGTH; i < N; ++i) {
        Graph<int, int>::Node newNode;
        addEdge(newNode, randomElement(G));
        G.addNodes({newNode});
    }

    G[0].data() = 0;
    for (int i = 1; i < N; ++i)
        G[i].data() = randomInt(0, MODULO - 1);

    G.Index(1); // randomize the nodes now
    for (auto &edge : G.edges())
        edge.data() = randomInt(1, MODULO - 1);

    cout << G.size() << " " << MODULO << "\n";
    for (auto &edge : shuffle(G.edges())) {
        int x = edge.from().index();
        int y = edge.to().index();
        int w = edge.data();
        if (randomElement({true, false}))
            swap(x, y);
        cout << x << " " << y << " " << w << "\n";
    }

    for (auto &node : G) {
        int gradient = 0;
        for (auto &edge : node.edges())
            gradient = (gradient + (edge.to().data() - node.data()) * edge.data()) % MODULO;
        gradient = (gradient + MODULO) % MODULO;
        cout << gradient << " ";
    }
    cout << "\n";
}
