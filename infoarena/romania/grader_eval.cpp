#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

void result(const string& message, int score) {
    cerr << message;
    cout << score;
    exit(0);
}

int main() {
    ifstream in("romania.in");
    ifstream out("romania.out");

    if (!out.is_open())
        result("Fisier de iesire lipsa", 0);

    int N, K; in >> N >> K;
    vector<int> degree(N, 0);
    for (int i = 0; i < K; ++i) {
        int x; in >> x;
        degree[x - 1]++;
    }

    vector<int> output_degree(N, 0);
    vector< pair<int, int> > edges(K);
    for (int i = 0; i < K; ++i) {
        if (!(out >> edges[i].first >> edges[i].second))
            result("Fisier de iesire incomplet", 0);
        if (edges[i].first < 1 || edges[i].first > N || edges[i].second < 1 || edges[i].second > N)
            result("Wrong answer", 0);
        if (edges[i].first == edges[i].second)
            result("Wrong answer", 0);
        if (edges[i].first == edges[i].second % N + 1 || edges[i].second == edges[i].first % N + 1)
            result("Wrong answer", 0);
        ++output_degree[edges[i].first - 1];
    }

    if (degree != output_degree)
        result("Wrong answer", 0);

    for (auto &edge : edges)
        if (edge.first > edge.second)
            swap(edge.first, edge.second);

    sort(edges.begin(), edges.end(), [&](pair<int, int> A, pair<int, int> B) {
        if (A.second != B.second)
            return A.second < B.second;
        return A.first > B.first;
    });

    vector< pair<int, int> > stack;
    for (auto &e : edges) {
        while (true) {
            if (!stack.size()) {
                stack.push_back(e);
                break;
            }

            if (stack.back() == e)
                result("Wrong answer", 0);
            if (stack.back().second <= e.first) {
                stack.push_back(e);
                break;
            }
            if (stack.back().first < e.first)
                result("Wrong answer", 0);
            stack.pop_back();
        }
    }
    result("OK", 10);
}
