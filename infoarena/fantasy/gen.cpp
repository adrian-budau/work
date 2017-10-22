#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <inputGenerator.hpp>

using namespace std;
using namespace inputGenerator;

int main (int argc, char **argv) {
    ios::sync_with_stdio(false);
    vector<string> arguments(argv + 1, argv + argc);

    if (arguments.size () < 3) {
        cout << "USAGE T MAXN MAXWIDE\n";
        return 1;
    }

    int T = stoi(arguments[0]);
    int MAXN = stoi(arguments[1]);
    int MAXWIDE = stoi(arguments[2]);

    vector<int> types = {0};
    for (int i = 3; i < int(arguments.size()); ++i)
        types.push_back(stoi(arguments[i]));

    cout << T << "\n";
    for (int test = 0; test < T; ++test) {
        int N = randomInt(11, MAXN);
        int WIDE = randomInt(9, min(N - 2, MAXWIDE));
        // Case 1 is either on the chain and cant get out on time, or outside and cant get saf eon time
        // Case 2 is when the distance between the dragon and knight is even and the only place for the wizard
        // to hide is starting from that middle
        // Case 3 is when we cant escape the queue, but there is literally no place where
        auto x = randomElement(types);
        cerr << x << " ";
        if (x > 0) {
            Graph<> T;
            vector<Node<>> attach;
            Node<> W, K, D;
            if (x == 1) {
                T = chain(randomInt(4, WIDE), Boolean::False);
                for (int i = 0; i < T.size() / 2; ++i)
                    attach.push_back(T[i]);

                D = T[0];
                K = T[T.size() - 1];
                int where = randomInt(2, T.size() - 2);
                if (where % 2 != (T.size() - 1) % 2)
                    --where;
                W = T[where];
                // now make sure that at least 1 of each exists
                int to_attach = randomInt(1, N - T.size());
                int where_to_attach = (where + T.size() - 1) / 2;
                for (int i = 0; i < to_attach; ++i) {
                    Node<> v;
                    addEdge(T[where_to_attach], v);
                    T.addNodes({v});
                    attach.push_back(v);
                }
            } else if (x == 2) {
                T = chain(randomInt(1, WIDE / 2) * 2 + 1, Boolean::False);
                for (int i = 0; i < T.size() / 2; ++i)
                    attach.push_back(T[i]);
                D = T[0];
                K = T[T.size() - 1];
                int to_attach = randomInt(1, N - T.size());
                int where_to_attach = T.size() / 2;
                vector<Node<>> now; now.push_back(T[where_to_attach]);
                for (int i = 0; i < to_attach; ++i) {
                    Node<> v;
                    addEdge(randomElement(now), v);
                    T.addNodes({v});
                    now.push_back(v);
                    attach.push_back(v);
                }
                W = randomElement(now);
            } else if (x == 3) {
                T = chain(randomInt(4, WIDE), Boolean::False);
                for (int i = 0; i < T.size() / 2; ++i)
                    attach.push_back(T[i]);
                D = T[0];
                K = T[T.size() - 1];
                int where_W = randomInt(1, T.size() - 2);
                W = T[where_W];
                for (int i = (where_W + T.size() - 1) / 2; i < T.size(); ++i)
                    attach.push_back(T[i]);
            } else if (x == 4) {
                T = chain(randomInt(9, WIDE), Boolean::False);
                for (int i = 0; i < T.size() / 2; ++i)
                    attach.push_back(T[i]);

                D = T[0];
                K = T[T.size() - 1];

                int where_chain = randomInt(3, T.size() / 2 - 1);
                int length = randomInt(1, min(N - T.size() - 1, where_chain - 2));
                int where_to_attach = (where_chain - length + T.size() - 1) / 2;
                if ((length + T.size() - 1 - where_chain) % 2)
                    ++length;
                auto second_chain = chain(length, Boolean::False);
                W = second_chain[length - 1];
                addEdge(second_chain[0], T[where_chain]);
                for (auto &node : second_chain)
                    T.addNodes({node});
                for (auto &node : second_chain)
                    attach.push_back(node);

                int to_attach = randomInt(1, N - T.size());
                for (int i = 0; i < to_attach; ++i) {
                    Node<> v;
                    addEdge(T[where_to_attach], v);
                    T.addNodes({v});
                    attach.push_back(v);
                }
            }

            while (T.size() < N) {
                Node<> v;
                addEdge(v, randomElement(attach));
                T.addNodes({v});
                attach.push_back(v);
            }

            T.Index(1);

            cout << N << " " << D.index() << " " << K.index() << " " << W.index() << "\n";
            for (auto &e : shuffle(T.edges())) {
                int x = e.from().index();
                int y = e.to().index();
                if (randomElement({true, false}))
                    swap(x, y);
                cout << x << " " << y << "\n";
            }
            continue;
        }

        Seed::logging = false;
        auto T = wideTree(N, WIDE);
        T.Index(1);
        auto three = shuffle(randomSubsequence(vector<Node<>>(T.begin(), T.end()), 3));
        int W = three[0].index();
        int K = three[1].index();
        int D = three[2].index();

        cout << N << " " << D << " " << K << " " << W << "\n";
        for (auto &e : shuffle(T.edges())) {
            int x = e.from().index();
            int y = e.to().index();
            if (randomElement({true, false}))
                swap(x, y);
            cout << x << " " << y << "\n";
        }
    }
}
