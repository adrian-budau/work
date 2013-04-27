#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

class Tree {
  public:
    Tree(const size_t &size):
            size_(size + 1),
            sons_(size + 1) {  // we index from 1, yeah i hate it
    }

    void addEdge(const int &father, const int &son) {
        sons_[father].push_back(son);
    }

    void prepare() {
        mapped_.resize(size_);
        parent_.resize(size_, 0);
        group_.resize(size_, 0);
        leader_.resize(size_, 0);

        int order = 0;

        // remap nodes
        dfs(root, [this, &order](const size_t &node) {
            mapped_[node] = ++order;
        }, [](const size_t&){});

        for (auto &edges: sons_)
            for (auto &son: edges)
                son = mapped_[son];

        // make runs
        dfs(root, [this](const size_t &node) {
            for (const auto &son: sons_[node])
                 parent_[son] = node;
        }, [this](const size_t &node) {
            group_[node] = node;
            for (const auto &son: sons_[node])
                if (trailingZeroes(group_[son]) > trailingZeroes(group_[node]))
                    group_[node] = group_[son];
            leader_[group_[node]] = node;
        });

        // get viable parent runs
        dfs(root, [this](const size_t &node) {
            if (!parent_[node]) {
                existing_[node] = (1 << trailingZeroes(group_[node]));
                return;
            }

            existing_[node] = existing_[parent_[node]];
            if (group_[parent_[node]] != group_[node])
                existing_[node] |= (1 << trailingZeroes(group_[node]));
        }, [this](const size_t&){});
    }

  private:
    template<class Before, class After>
    void dfs(const size_t &node, Before B, After A) {
        B(node);

        for (const auto &son: sons_[node])
            dfs(son, B, A);

        A(node);
    }

    size_t trailingZeroes(size_t value) {
        return __builtin_ctz(value);
    }

    static const size_t root = 1;
    size_t size_;
    vector< vector<size_t> > sons_;

    vector<size_t> mapped_;
    vector<size_t> group_;
    vector<size_t> parent_;
    vector<size_t> leader_;

    vector<size_t> existing_;
};

int main() {
    ifstream cin("lca.in");
    ofstream cout("lca.out");

    int N, M; cin >> N >> M;

    Tree T(N);
    for (int i = 1; i < N; ++i) {
        int node; cin >> node;
        T.addEdge(node, i);
    }

    T.prepare();

    vector<int> answer(M);
    for (int i = 0; i < M; ++i) {
        int x, y; cin >> x >> y;
        cout << T.query(x, y) << "\n";
    }

}
