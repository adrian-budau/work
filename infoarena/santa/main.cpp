#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <functional>
#include <stack>
#include <map>
#include <queue>

using namespace std;

template<class T = int>
class Graph {
  public:
    Graph(int size = 0):
            edges_(size) {
    }

    void addEdge(int x, T y) {
        edges_[x].push_back(y);
    }

    vector<int> pathBetween(int from, int to) {
        vector<int> answer(1, from);
        vector<bool> used(edges_.size(), false);
        used[from] = true;
        if (not backtrack(from, to, answer, used))
            return vector<int>();
        return answer;
    }

  protected:
    vector< vector<T> > edges_;

  private:
    bool backtrack(int from, int to, vector<int>& answer, vector<bool> &used) {
        if (to != -1 and answer.size() + 1 == edges_.size()) {
            auto it = find(edges_[from].begin(), edges_[from].end(), to);
            if (it != edges_[from].end()) {
                answer.push_back(to);
                return true;
            }
            return false;
        }

        if (answer.size() == edges_.size()) {
            if (from == to or to == -1)
                return true;
            return false;
        }

        for (auto &node : edges_[from])
            if (not used[node] and node != to) {
                used[node] = true;
                answer.push_back(node);
                if (backtrack(node, to, answer, used))
                    return true;
                answer.pop_back();
                used[node] = false;
            }
        return false;
    }
};


int findByFirst(const vector< pair<int, int> > &data, int value) {
    return find_if(data.begin(), data.end(), [&value](pair<int, int> a) {
        return a.first == value;
    }) -> second;
}


class ComponentGraph final : public Graph<pair<int, int> > {
  public:
    ComponentGraph() {
    }

    ComponentGraph(const vector<Graph<>> &components):
            Graph(components.size()),
            components_(components) {
    }

    vector<int> findChain(int start, const vector<int>& ends) {
        queue<int> Q;
        vector<int> from(edges_.size(), -1);
        from[start] = start;
        Q.push(start);

        vector<bool> ending_node(edges_.size(), false);
        for (auto &node : ends)
            ending_node[node] = true;
        while (!Q.empty()) {
            int node = Q.front();
            Q.pop();

            if (ending_node[node]) { // yey we found the chain
                vector<int> answer;
                for (; node != start; node = from[node])
                    answer.push_back(node);
                answer.push_back(start);
                reverse(answer.begin(), answer.end());
                return answer;
            }

            for (auto &next : edges_[node])
                if (from[next.first] == -1) {
                    from[next.first] = node;
                    Q.push(next.first);
                }
        }

        return vector<int>();
    }

    int edgeBetween(int first_component, int second_component) {
        return findByFirst(edges_[first_component], second_component);
    }

    Graph<>& operator[](const int &index) {
        return components_[index];
    }

  private:
    vector<Graph<>> components_;
};

class SimpleGraph final : public Graph<> {
  public:
    SimpleGraph(int size = 0):
            Graph(size) {
    }

    pair<ComponentGraph, vector< vector< pair<int, int> > > > biconnectedComponents() {
        lowlink_ = depth_ = vector<int>(edges_.size(), 0);
        node_in_components_.resize(edges_.size());

        for (int i = 0; i < int(edges_.size()); ++i)
            if (!depth_[i]) {
                depth_[i] = 1;
                dfs(i);
            }

        ComponentGraph C(components_);
        for (int i = 0; i < int(edges_.size()); ++i)
            for (auto &first_component : node_in_components_[i])
                for (auto &second_component : node_in_components_[i])
                    if (first_component < second_component) {
                        C.addEdge(first_component.first, {second_component.first, i});
                        C.addEdge(second_component.first, {first_component.first, i});
                    }
        return {C, node_in_components_};
    }

  private:
    void dfs(int node) {
        lowlink_[node] = depth_[node];

        for (auto &next : edges_[node])
            if (!depth_[next]) {
                depth_[next] = depth_[node] + 1;
                stacked_edges_.push({node, next});
                dfs(next);
                lowlink_[node] = min(lowlink_[next], lowlink_[node]);

                if (lowlink_[next] >= depth_[node]) // new component yey
                    popComponent(node, next);
            } else if (depth_[next] < depth_[node] - 1) { // if it's an node -> ancestor edge and the ancestor is not the father
                stacked_edges_.push({node, next}); 
                lowlink_[node] = min(lowlink_[node], depth_[next]);
            }
    }

    void popComponent(int from, int to) {
        vector<int> nodes;
        vector< pair<int, int> > edges;

        int poped_from, poped_to;
        do {
            tie(poped_from, poped_to) = stacked_edges_.top();
            stacked_edges_.pop();

            edges.push_back({poped_from, poped_to});
            nodes.push_back(poped_from);
            nodes.push_back(poped_to);
        } while (poped_from != from or poped_to != to);

        sort(nodes.begin(), nodes.end());
        nodes.erase(unique(nodes.begin(), nodes.end()), nodes.end());

        // let's normalize the graph
        map<int, int> M;
        for (int i = 0; i < int(nodes.size()); ++i) {
            M[nodes[i]] = i;
            node_in_components_[nodes[i]].push_back({components_.size(), i});
        }

        Graph G(nodes.size());
        for (auto &edge : edges) {
            G.addEdge(M[edge.first], M[edge.second]);
            G.addEdge(M[edge.second], M[edge.first]);
        }

        components_.push_back(G);
    }

    vector<int> lowlink_, depth_;

    vector< vector< pair<int, int> > > node_in_components_;

    stack< pair<int, int> > stacked_edges_;

    vector<Graph<>> components_;
};

vector< pair<int, int> >::const_iterator commonElement(const vector<pair<int, int>> &first, const vector<pair<int, int>> &second) {
    vector< pair<int, int> >::const_iterator first_iterator = first.begin(), second_iterator = second.begin();

    while (first_iterator != first.end()) {
        while (second_iterator != second.end() and second_iterator->first < first_iterator->first)
            ++second_iterator;
        if (second_iterator->first == first_iterator->first)
            return first_iterator;
        ++first_iterator;
    }

    return first_iterator;
}

vector<int> removeSecond(const vector< pair<int, int> >& data) {
    vector<int> answer(data.size());
    transform(data.begin(), data.end(), answer.begin(), [](pair<int, int> p) {
        return p.first;
    });
    return answer;
}

int main() {
    ifstream cin("santa.in");
    ofstream cout("santa.out");

    int N, M; cin >> N >> M;

    SimpleGraph G(N);
    for (int i = 0; i < M; ++i) {
        int x, y; cin >> x >> y;
        G.addEdge(x - 1, y - 1);
        G.addEdge(y - 1, x - 1);
    }

    int start, end, bad_guy; cin >> start >> end >> bad_guy;
    --start; --end; --bad_guy;
    ComponentGraph C;
    vector< vector< pair<int, int> > > nodeInComponents;
    tie(C, nodeInComponents) = G.biconnectedComponents();

    // let's find the components that need checking
    // let's see where the bad guy is, in the start component or end component
    int startComponent;
    if (commonElement(nodeInComponents[start], nodeInComponents[bad_guy]) != nodeInComponents[start].end()) {
        startComponent = commonElement(nodeInComponents[start], nodeInComponents[bad_guy]) -> first;
    } else if (commonElement(nodeInComponents[end], nodeInComponents[bad_guy]) != nodeInComponents[end].end()) {
        startComponent = commonElement(nodeInComponents[end], nodeInComponents[bad_guy]) -> first;
        swap(start, end);
    } else {
        cout << "No chance0.5\n";
        return 0;
    }

    auto chain = C.findChain(startComponent, removeSecond(nodeInComponents[end])); // we want to stop at any component that contains the end node

    if (chain.empty()) {
        cout << "No chance1\n";
        return 0;
    }

    // yey now we have little left
    vector< pair<int, int> > answer;
    int last = bad_guy;
    for (int i = 1; i < int(chain.size()); ++i) {
        int node = C.edgeBetween(chain[i - 1], chain[i]);

        auto path = C[chain[i - 1]].pathBetween(
            findByFirst(nodeInComponents[last], chain[i - 1]),
            findByFirst(nodeInComponents[node], chain[i - 1])
        );

        if (path.empty()) {
            cout << "No chance2\n";
            return 0;
        }

        path.pop_back(); // we need this to not repeat nodes
        for (auto &node : path)
            answer.push_back({chain[i - 1], node}); // we need to remap them in the end
        last = node;
    }

    auto path = C[chain.back()].pathBetween(findByFirst(nodeInComponents[last], chain.back()), -1);
    if (path.empty()) {
        cout << "No chance3\n";
        return 0;
    }

    for (auto &node : path)
        answer.push_back({chain.back(), node});
    // we need to invert the nodeInComponents in the end
    map< pair<int, int>, int> remap;
    for (int i = 0; i < N; ++i)
        for (auto &p : nodeInComponents[i])
            remap[p] = i;

    cout << answer.size() << "\n";
    for (auto &node : answer)
        cout << remap[node] + 1 << " ";
    cout << "\n";
}
