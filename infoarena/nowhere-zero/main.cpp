#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cassert>
#include <queue>
#include <array>

using namespace std;

class Edge {
  public:
    int from, to, index;
};

int main() {
    ifstream cin("nowherezero.in");
    ofstream cout("nowherezero.out");

    int N, M;
    cin >> N >> M;

    vector<double> X(N), Y(N);
    for (int i = 0; i < N; ++i)
        cin >> X[i] >> Y[i];

    vector< vector<Edge> > E(N);
    for (int i = 0; i < M; ++i) {
        int x, y; cin >> x >> y;
        --x; --y;
        E[x].push_back({x, y, i});
        E[y].push_back({y, x, i});
    }

    vector<int> in_first(M, -1), in_second(M, -1);

    auto where = [&](const Edge& edge, int &who) -> vector<int>& {
        if (who == min(edge.from, edge.to))
            return in_first;
        return in_second;
    };

    for (int i = 0; i < N; ++i) {
        sort(E[i].begin(), E[i].end(), [&](const Edge &x, const Edge &y) {
            int a = x.to;
            int b = y.to;
            return atan2(Y[a] - Y[i], X[a] - X[i]) < atan2(Y[b] - Y[i], X[b] - X[i]);
        });

        for (int j = 0; j < int(E[i].size()); ++j) {
            // we set for each edge on what position do we find it in a list
            where(E[i][j], i)[E[i][j].index] = j;
        }
    }

    // now we need to get for each edge its face
    // we start with each that hasn't been used twice(for both sides)
    // and go round a circle until we reach the starting point

    vector<int> used_times(M, 0);       // how many times have we used this edge, each bit represents an orientation
    vector< vector<int> > faces(M);     // the faces on the left and on the right
                                        // on this task they should be different for each edge

    vector< vector< pair<int, int> > > orientation(M); // how each edge is oriented in each face
    int faces_number = 0;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < int(E[i].size()); ++j)
            if (used_times[E[i][j].index] < 3) { // we have a new face starting from here, going next to the right and so on
                int start = i; // when we reach start its a face
                int node = i;  // where we are now
                int which = j; // which edge are we won
                int next = E[node][which].to;

                if (node < next and used_times[E[node][which].index] & 1) // we've used this orientation
                    continue;
                if (next < node and used_times[E[node][which].index] & 2) // or this orientation
                    continue;
                do {
                    orientation[E[node][which].index].push_back({E[node][which].from, E[node][which].to});
                    faces[E[node][which].index].push_back(faces_number);                        // this is one of the faces of this edge

                    int next_node = E[node][which].to;                                          // next node
                    if (node < next_node)                                                       // we have one less trip through this edge
                        used_times[E[node][which].index] |= 1;
                    else
                        used_times[E[node][which].index] |= 2;      
                    int next_which = where(E[node][which], next_node)[E[node][which].index];    // next location of this edge
                    next_which = (next_which + 1) % E[next_node].size();                        // we go to the right

                    node = next_node;
                    which = next_which;
                } while (node != start); // until we get to the start
                faces_number++; // we found one more face
            }

    // now we have for each the faces
    vector< vector<int> > face_edges(faces_number);
    vector<int> face_degree(faces_number, 0);
    for (int i = 0; i < M; ++i) {
        assert(faces[i].size() == 2);
        assert(faces[i][0] != faces[i][1]); // it's a biconnected planar graph
        int first = faces[i][0];
        int second = faces[i][1];
        face_edges[first].push_back(second);
        face_edges[second].push_back(first);
    }

    for (int i = 0; i < faces_number; ++i) {
        sort(face_edges[i].begin(), face_edges[i].end());
        face_edges[i].erase(unique(face_edges[i].begin(), face_edges[i].end()), face_edges[i].end());
        face_degree[i] = face_edges[i].size();
    }

    queue<int> Q;       // the queue with the faces with degree < 6
    vector<int> order;  // the order in which we remove the faces
                        // we take it reverse to find the colors of each face
    for (int i = 0; i < faces_number; ++i)
        if (face_degree[i] < 6) {
            face_degree[i] = -1;
            Q.push(i);
            order.push_back(i);
        }

    while (!Q.empty()) {
        int face = Q.front();
        Q.pop();
        for (auto &next_face : face_edges[face]) {
            --face_degree[next_face];
            if (face_degree[next_face] >= 0 and face_degree[next_face] < 6) {
                face_degree[next_face] = -1;
                Q.push(next_face);
                order.push_back(next_face);
            }
        }
    }

    reverse(order.begin(), order.end());
    vector<int> colors(faces_number, 0);

    for (auto &face : order) {
        array<int, 7> used({0, 0, 0, 0, 0, 0, 0}); // what colors our neighbours have
        for (auto &neighbour : face_edges[face])
            used[colors[neighbour]] = 1;
        for (int i = 1; i <= 6; ++i)
            if (!used[i]) {
                colors[face] = i;
                break;
            }
    }

    // now we have coloured the faces
    // we need the circulation
    auto abs = [](const int &value) {
        if (value < 0)
            return -value;
        return value;
    };

    for (int i = 0; i < M; ++i) {
        pair<int, int> way;
        if (colors[faces[i][0]] > colors[faces[i][1]])
            way = orientation[i][0];
        else
            way = orientation[i][1];

        int capacity = abs(colors[faces[i][0]] - colors[faces[i][1]]);
        assert(capacity != 0);
        cout << way.first + 1 << " " << way.second + 1 << " " << capacity << "\n";
    }
}
