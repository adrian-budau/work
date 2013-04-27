#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

class Tree {
  public:
    Tree(const size_t &size):
            size_(size),
            sons_(size),
            queries_(size) {
    }

    void addEdge(const int &father, const int &son) {
        sons_[father].push_back(son);
    }

    void solve() {
        times_ = vector<size_t>(size_, NOT_DONE);
        current_time = 0;

        stack_.clear();
        stack_.reserve(size_);
        dfs(root);
    }

    void addQuery(const int &x, const int &y, int &answer) {
        if (times_[x] < times_[y])
            queries_[y].push_back(Query(x, answer));
        else
            queries_[x].push_back(Query(y, answer));
    }

  private:
    static const size_t root = 0;

    enum {
        NOT_DONE = 0
    };

    class Query {
      public:
        Query(const int &other_node, int& answer):
            other_(other_node),
            answer_(&answer) {
        }

        operator int() {
            return other_;
        }

        int& operator=(const int &value) {
            return(*answer_ = value);
        }

      private:
        int other_;
        int* answer_;
    };

    void dfs(const int &node) {
        times_[node] = ++current_time;
        stack_.push_back(node);

        for (vector<size_t>::const_iterator it = sons_[node].begin(); it != sons_[node].end(); ++it)
            dfs(*it);

        for (vector<Query>::iterator it = queries_[node].begin(); it != queries_[node].end(); ++it) {
            if (*it == node)
                *it = node;
            // binary search
            size_t step, position = stack_.size() - 1;
            for (step = 1; step < stack_.size(); step <<= 1)
                if (times_[stack_[position - step]] < times_[*it])
                    break;

            for (; step; step >>= 1)
                if (step < position && times_[stack_[position - step]] > times_[*it])
                    position -= step;
            if (times_[stack_[position]] > times_[*it])
                --position;
            *it = stack_[position];
        }

        times_[node] = ++current_time;
        stack_.pop_back();
    }

    size_t size_;
    vector< vector<size_t> > sons_;
    vector< vector<Query> > queries_;

    size_t current_time;
    vector<size_t> times_;
    vector<size_t> stack_;
};

int main() {
    ifstream cin("lca.in");
    ofstream cout("lca.out");

    int N, M; cin >> N >> M;

    Tree T(N);
    for (int i = 1; i < N; ++i) {
        int node; cin >> node;
        T.addEdge(node - 1, i);
    }

    T.solve();  // just for times

    vector<int> answer(M);
    for (int i = 0; i < M; ++i) {
        int x, y; cin >> x >> y;
        T.addQuery(x - 1, y - 1, answer[i]);
    }

    T.solve();
    for (int i = 0; i < M; ++i)
        cout << answer[i] + 1 << "\n";
}
