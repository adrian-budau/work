#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

int kKeys = 200;

class Graph {
  public:
    explicit Graph(const int &size):
        edges_(size),
        marked_(size, false) {
    }

    void addEdge(const int &from, const int &to) {
        edges_[from].push_back(to);
    }

    vector<int> tryLoop(const int &root) {
        vector<int> from(edges_.size(), -1);
        queue<int> Q;

        Q.push(root);
        while (!Q.empty()) {
            int node = Q.front();
            Q.pop();
            if (from[root] != -1)
                continue;
            for (auto &neighbour : edges_[node])
                if (from[neighbour] == -1 && marked_[neighbour] == false) {
                    from[neighbour] = node;
                    Q.push(neighbour);
                }
        }

        if (from[root] == -1)
            return vector<int>();

        vector<int> answer;
        for (int i = from[root]; i != root; i = from[i])
            answer.push_back(i);
        return answer;
    }

    void markNode(const int &node) {
        marked_[node] = true;
    }

    vector<int> notMarked(const int &node) {
        vector<int> answer;
        for (auto &neighbour : edges_[node])
            if (not marked_[neighbour])
                answer.push_back(neighbour);
        return answer;
    }

  private:
    vector< vector<int> > edges_;
    vector<bool> marked_;
};

class State {
  public:
    explicit State(const int &size):
        many_(kKeys, 0),
        type_(size),
        keys_(size),
        opened_(size, false) {
    }

    void addKey(const int &key) {
        many_[key]++;
    }

    void setType(const int &chest, const int &keyType) {
        type_[chest] = keyType;
    }

    void addChestKey(const int &chest, const int &key) {
        keys_[chest].push_back(key);
    }

    bool open(const int &chest) {
        if (opened_[chest] || many_[type_[chest]] == 0)
            return false;

        many_[type_[chest]] --;
        for (auto &key: keys_[chest])
            ++many_[key];

        opened_[chest] = true;
        return true;
    }

    operator bool() {
        Graph G(many_.size() + type_.size());
        queue<int> Q;
        vector<int> current_many_ = many_;
        for (int i = 0; i < int(type_.size()); ++i)
            if (not opened_[i]) {
                G.addEdge(type_[i], many_.size() + i);
                for (auto &key : keys_[i])
                    G.addEdge(many_.size() + i, key);
            } else {
                G.markNode(many_.size() + i);
            }

        for (int i = 0; i < int(current_many_.size()); ++i)
            if (current_many_[i] > 0) {
                Q.push(i);
            }

        queue<int> finished;
        vector<int> need(current_many_.size(), 0);
        for (int i = 0; i < int(current_many_.size()); ++i) {
            need[i] = G.notMarked(i).size();
            if (current_many_[i] >= need[i] && need[i]) {
                need[i] = 0;
                finished.push(i);
            }
        }

        while (!Q.empty() || !finished.empty()) {
            auto open = [&](const int &chest) {
                if (need[type_[chest]] != 0)
                    need[type_[chest]] --;
                for (auto &key : keys_[chest]) {
                    if (current_many_[key]++ == 0 && need[key])
                        Q.push(key);
                    if (current_many_[key] >= need[key] + static_cast<int>(type_[chest] == key) && need[key]) {
                        need[key] = 0;
                        finished.push(key);
                    }
                }
            };

            while (!finished.empty()) {
                int keyType = finished.front();
                finished.pop();
                auto chests = G.notMarked(keyType);
                for (auto &chest : chests) {
                    G.markNode(chest);
                    open(chest - many_.size());
                    --current_many_[type_[chest - many_.size()]];
                }
            }

            if (Q.empty())
                continue;
            int keyType = Q.front();
            auto passedNodes = G.tryLoop(keyType);

            if (not passedNodes.size()) {
                Q.pop();
                continue;
            }

            for (auto &value : passedNodes) {
                if (value >= int(many_.size())) { // it's a chest
                    G.markNode(value);  // so we can't use it
                    open(value - many_.size());
                    --current_many_[type_[value - many_.size()]];  // we got it back anyway
                }
            }
        }

        if (*max_element(need.begin(), need.end()) > 0)
            return false;
        return true;
    }

  private:
    vector<int> many_;
    vector<int> type_;
    vector<vector<int>> keys_;

    vector<bool> opened_;
};

int main() {
    int T; cin >> T;

    for (int i = 1; i <= T; ++i) {
        int K, N; cin >> K >> N;

        State S(N);
        for (int i = 0; i < K; ++i) {
            int x; cin >> x;
            S.addKey(x - 1);
        }

        for (int i = 0; i < N; ++i) {
            int type, count; cin >> type >> count;
            S.setType(i, type - 1);

            for (int j = 0; j < count; ++j) {
                int x; cin >> x;
                S.addChestKey(i, x - 1);
            }
        }

        cout << "Case #" << i << ": ";
        if (not S) {
            cout << "IMPOSSIBLE\n";
            continue;
        }

        vector<int> answer;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                State next = S;
                if (!next.open(j))
                    continue;

                if (not next)
                    continue;

                S = next;
                answer.push_back(j + 1);
                break;
            }
        }

        for (auto &x : answer)
            cout << x << " ";
        cout << "\n";
    }
}
