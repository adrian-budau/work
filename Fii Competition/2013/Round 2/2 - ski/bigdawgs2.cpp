#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

class Graph {
  public:
    enum {
        TELEFERIC = 1,
        SKI_TRACK = 2
    };

    explicit Graph(const int &size = 1):
        data_(size, vector<double>(size, inf)) {
    }

    void addEdge(const int &from, const int &to, const double &cost) {
        if (cost < data_[from][to])
            data_[from][to] = cost;
    }

    vector<double> shortestPaths(const int &root) {
        vector<double> minimumDistance(data_.size(), inf);
        vector<bool> taken(data_.size(), false);

        minimumDistance[root] = 0;
        for (int i = 0; i < int(data_.size()); ++i) {
            double minDis = 2 * inf;
            int where = -1;
            for (int j = 0; j < int(data_.size()); ++j)
                if (taken[j] == false && minimumDistance[j] < minDis) {
                    minDis = minimumDistance[j];
                    where = j;
                }

            // let's expand
            int from = where;
            taken[from] = true;
            for (int k = 0; k < int(data_.size()); ++k)
                if (minDis + data_[from][k] < minimumDistance[k])
                    minimumDistance[k] = minDis + data_[from][k];
        }

        return minimumDistance;
    }

  private:
    const double inf = 1e50;
    vector< vector<double> > data_;
};

int main() {
    ifstream cin("ski.in");
    ofstream cout("ski.out");

    int N, M, I, V; cin >> N >> M >> I >> V;
    --I; --V;
    int VI, VV, VT; cin >> VI >> VV >> VT;
    Graph GI(N), GV(N);

    for (int i = 0; i < M; ++i) {
        int type, from, to, distance;
        cin >> type >> from >> to >> distance;
        --from; --to;

        if (type == Graph::TELEFERIC) {
            double time = 3600.0 * static_cast<double>(distance) / static_cast<double>(VT);

            GI.addEdge(from, to, time);
            GI.addEdge(to, from, time);
            GV.addEdge(from, to, time);
            GV.addEdge(to, from, time);
        } else {
            double timeI = 3600.0 * static_cast<double>(distance) / static_cast<double>(VI);
            double timeV = 3600.0 * static_cast<double>(distance) / static_cast<double>(VV);
            GI.addEdge(from, to, timeI);
            GV.addEdge(from, to, timeV);
        }
    }

    vector<double> dI = GI.shortestPaths(I);
    vector<double> dV = GV.shortestPaths(V);

    double answer = 1e50;
    int where = -1;
    for (int i = 0; i < N; ++i)
        if (max(dI[i],dV[i]) < answer) {
            answer = max(dI[i], dV[i]);
            where = i;
        }

    if (answer > 1e12) {
        cout << "-1\n";
        return 0;
    }

    cout << where + 1 << "\n";
    int time_required = int(floor(answer));

    cout << time_required / 3600 << ":";
    time_required %= 3600;
    cout << time_required / 60 << ":" << time_required % 60 << "\n";
}
