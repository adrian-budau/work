#include <fstream>
#include <inputGenerator.hpp>

using namespace std;
using namespace inputGenerator;

int main() {
    ofstream cout("diametru.out");

    auto g = undirectedGraph(500, 1500, Boolean::True);
    g.Index(1);
    cout << g.size() << " " << g.edges().size() << "\n";

    for (auto &e : g.edges()) {
        cout << e.from().index() << " " << e.to().index() << "\n";
    }
}
