#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <set>
#include <map>
#include <fstream>

using namespace std;

void result(const string &message, int score) {
    cerr << message;
    cout << score;
    exit(0);
}

int main() {
    ifstream in("nowhere-zero.in");

    if (!in.is_open())
        result("Fisier de intrare lipsa", 0);

    ifstream out("nowhere-zero.out");

    if (!out.is_open())
        result("Fisier de iesire lipsa", 0);

    int N, M;
    if (!(in >> N >> M))
        result("Fisier de intrare gresit, lipsteste N si/sau M", 0);

    for (int i = 0; i < N; ++i) {
        double x, y;
        if (!(in >> x >> y))
            result("Fisier de intrare gresit, lipsesc coordonate pentru un punct", 0);
    }

    set< pair<int, int> > E;
    for (int i = 0; i < M; ++i) {
        int from, to;
        if (!(in >> from >> to))
            result("Fisier de intrare gresit, lipsesc muchii", 0);
        if (from < 1 || from > N || to < 1 || to > N || from == to)
            result("Fiser de intrare gresit, muchie nepermisa", 0);
        if (from > to)
            swap(from, to);
        E.insert({from, to});
    }

    if (int(E.size()) != M)
        result("Fisier de intrare gresit, muchii duble", 0);

    vector<int> circulation(N, 0);
    for (int i = 0; i < M; ++i) {
        int from, to, capacity;
        if (!(out >> from >> to >> capacity))
            result("Fisier de iesire incomplet", 0);
        if (from < 1 || from > N || to < 1 || to > N || from == to || capacity < 1 || capacity > 5)
            result("Wrong answer", 0);

        circulation[from] += capacity;
        circulation[to] -= capacity;
        if (from > to)
            swap(from, to);
        if (E.find({from, to}) == E.end())
            result("Wrong answer", 0);
        E.erase({from, to});
    }

    if (circulation != vector<int>(N, 0))
        result("Wrong answer", 0);

    result("OK", 10);
} 
