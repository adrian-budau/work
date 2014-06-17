#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <array>

using namespace std;

const array<int, 8> dx{-1, -1, 0, 1, 1, 1, 0, -1},
                    dy{0, 1, 1, 1, 0, -1, -1, -1};

template<class T>
inline void minimize(T &a, T b) {
    if (b < a)
        a = b;
}

class Graph {
  public:
    Graph(int size):
            edge_(size, vector<int>(size, numeric_limits<int>::max())) {
    }

    void addEdge(int from, int to, int cost) {
        edge_[from][to] = cost;
    }

    int size() const {
        return edge_.size();
    }

    void dijkstraFrom(int source) {
        distance_ = vector<int>(size(), numeric_limits<int>::max());
        vector<bool> used_(size(), false);

        distance_[source] = 0;

        for (int i = 0; i < size(); ++i) {
            int best = numeric_limits<int>::max();
            int where = -1;
            for (int j = 0; j < size(); ++j)
                if (not used_[j] and distance_[j] < best) {
                    best = distance_[j];
                    where = j;
                }

            used_[where] = true;
            for (int j = 0; j < size(); ++j)
                if (distance_[j] - distance_[where] > edge_[where][j])
                    distance_[j] = distance_[where] + edge_[where][j];
        }
    }

    int distanceTo(int to) const {
        return distance_[to];
    }

  private:
    vector< vector<int> > edge_;

    vector<int> distance_;
};

int64_t best(const vector< array< array< array< array<int64_t, 8>, 5>, 8>, 5> >& distance, int64_t TMax) {
    array< array<int64_t, 8>, 5> now_distance;
    for (int i = 0; i < 5; ++i)
        for (int j = 0; j < 8; ++j)
            if (i == 2)
                now_distance[i][j] = 0;
            else
                now_distance[i][j] = numeric_limits<int64_t>::max() / 2;

    int64_t answer = 0;
    for (int i = distance.size() - 1; i >= 0; --i) {
        array< array<int64_t, 8>, 5> next_distance;

        for (int row = 0; row < 5; ++row)
            for (int direction = 0; direction < 8; ++direction) {
                next_distance[row][direction] = numeric_limits<int64_t>::max() / 2;
                for (int from_row = 0; from_row < 5; ++from_row)
                    for (int from_direction = 0; from_direction < 8; ++from_direction)
                        minimize(next_distance[row][direction],
                                 now_distance[from_row][from_direction] + distance[i][from_row][from_direction][row][direction]);
            }

        int64_t lowest_distance = next_distance[0][0];
        for (int row = 0; row < 5; ++row)
            for (int direction = 0; direction < 8; ++direction)
                minimize(lowest_distance, next_distance[row][direction]);
        if (lowest_distance <= TMax) {
            answer += (1LL << i);
            swap(now_distance, next_distance);
        }
    }
    return answer;
};

int main() {
    ifstream cin("biomech.in");
    ofstream cout("biomech.out");

    int64_t TMax; cin >> TMax;

    vector< vector<int> > directionChange(8, vector<int>(8));
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            cin >> directionChange[i][j];

    vector< vector<int> > lineAdvance(5, vector<int>(8));
    for (int i = 0; i < 5; ++i)
        for (int j = 0; j < 8; ++j)
            cin >> lineAdvance[i][j];

    /**
     * We can most 9 to the left and 9 to the right, or something like that
     * We move left continously 8 times then down-right 4 times then up-right 4 times
     * This looks like an edge case
     *
     * Hence 19 (our column + 9 left + 9 right)
     * 5 rows
     * 19 columns
     * 8 directions
     */
    auto graphState = [](int line, int column, int direction) {
        return line * 19 * 8 + column * 8 + direction;
    };

    Graph G(5 * 19 * 8);
    for (int row = 0; row < 5; ++row)
        for (int col = 0; col < 19; ++col)
            for (int direction = 0; direction < 8; ++direction) {
                for (int new_direction = 0; new_direction < 8; ++new_direction)
                    if (direction != new_direction)
                        G.addEdge(graphState(row, col, direction),
                                  graphState(row, col, new_direction),
                                  directionChange[direction][new_direction]); // the edge of changing direction

                int new_row = row + dx[direction];
                int new_col = col + dy[direction];
                if (new_row < 0 or new_row > 4)
                    continue;
                if (new_col < 0 or new_col > 18)
                    continue;
                G.addEdge(graphState(row, col, direction),
                        graphState(new_row, new_col, direction),
                        lineAdvance[row][direction]); // the edge for advancing in a direction
            }

    /**
     * left[log][row][dir][next_row][next_dir] is the minimum time to move 
     *      2^log columns to the left from row and dir
     *      to new_row and new_dir
     * same for right
     */
    vector<array< array< array< array<int64_t, 8>, 5>, 8>, 5>> left(1), right(1);

    for (int row = 0; row < 5; ++row)
        for (int direction = 0; direction < 8; ++direction) {
            G.dijkstraFrom(graphState(row, 9, direction)); // from the middle
            for (int new_row = 0; new_row < 5; ++new_row)
                for (int new_direction = 0; new_direction < 8; ++new_direction) {
                    left[0][row][direction][new_row][new_direction] =
                        G.distanceTo(graphState(new_row, 8, new_direction));
                    right[0][row][direction][new_row][new_direction] =
                        G.distanceTo(graphState(new_row, 10, new_direction));
                }
        }

    for (int i = 1; (1LL << i) <= TMax; ++i) {
        left.push_back(left.front());
        right.push_back(right.front());
        for (int row = 0; row < 5; ++row)
            for (int direction = 0; direction < 8; ++direction)
                for (int next_row = 0; next_row < 5; ++next_row)
                    for (int next_direction = 0; next_direction < 8; ++next_direction) {
                        left[i][row][direction][next_row][next_direction] = 
                        right[i][row][direction][next_row][next_direction] =
                            numeric_limits<int64_t>::max() / 2;
                        for (int mid_row = 0; mid_row < 5; ++mid_row)
                            for (int mid_direction = 0; mid_direction < 8; ++mid_direction) {
                                minimize(left[i][row][direction][next_row][next_direction],
                                         left[i - 1][row][direction][mid_row][mid_direction] + left[i - 1][mid_row][mid_direction][next_row][next_direction]);
                                minimize(right[i][row][direction][next_row][next_direction],
                                         right[i - 1][row][direction][mid_row][mid_direction] + right[i - 1][mid_row][mid_direction][next_row][next_direction]);
                            }
                    }
    }

    /* binary search answer */
    cout << max(best(left, TMax), best(right, TMax)) << "\n";
}
