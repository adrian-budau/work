#include <iostream>
#include <fstream>
#include <algorithm>
#include <inputGenerator.hpp>

using namespace std;
using namespace inputGenerator;

void dfsTopological(Node< pair<int, bool> > node, int& index, Node<pair<int, bool> > father = Node< pair<int, bool> >()) {
    node.data().first = index++;
    for (auto &neighbour : node.edges()) {
        if (neighbour.to() == father)
            continue;
        dfsTopological(neighbour.to(), index, node);
    }
}

vector< vector<int> > randomStronglyConnectedComponent(int size) {
    vector< vector<int> > edges(size);
    for (int i = 0; i < size; ++i)
        edges[i].push_back((i + 1) % size);
    return edges;
}

int main(int argc, char **argv) {
    vector<string> arguments(argv + 1, argv + argc);

    if (arguments.size() < 4) {
        cerr << "N SIGMA COMPONENTS MINEDGES\n";
        return 1;
    }

    int N = stoi(arguments[0]);
    int SIGMA = stoi(arguments[1]);
    int COMPONENTS = stoi(arguments[2]);
    int MINEDGES = stoi(arguments[3]);

    //Seed::logging = false;
    if (arguments.size() > 4) {
        reSeed(stoul(arguments[4]));
    }

    auto sizes = randomPartition(N, COMPONENTS);
    // the range in which the sources and destinations are put
    int length = randomInt(1, COMPONENTS - 2);
    int source_component = randomInt(1, COMPONENTS - length - 1);

    // how are they distributed here? a freaking tree
    auto T = tree< pair<int, bool> >(length);
    vector<bool> has_final(COMPONENTS, false);
    for (int i = 0; i < length; ++i)
        T[i].data().second = randomElement({true, false});
    if (randomInt(1, 10) == 1)
        for (int i = 0; i < length; ++i)
            T[i].data().second = false;
    bool exists_final = false;
    for (int i = 0; i < length; ++i)
        if (T[i].data().second)
            exists_final = true;
    if (!exists_final)
        has_final[randomInt(source_component, source_component + length - 1)] = true;

    int index = 0;
    vector< vector<int> > component_edges(COMPONENTS);
    dfsTopological(T[0], index);
    for (int i = 0; i < length; ++i) {
        if (T[i].data().second)
            has_final[T[i].data().first + source_component] = true;
        for (auto node : T[i].edges())
            if (node.to().data().first > T[i].data().first)
                component_edges[T[i].data().first + source_component].emplace_back(node.to().data().first + source_component);
    }

    vector<bool> is_possible(COMPONENTS, false);
    for (int i = source_component + length - 1; i >= source_component; --i) {
        if (has_final[i]) {
            is_possible[i] = true;
            continue;
        }
        for (auto &next : component_edges[i])
            if (is_possible[next])
                is_possible[i] = true;
    }

    vector<int> possible;
    for (int i = 0; i < COMPONENTS; ++i)
        if (is_possible[i])
            possible.push_back(i);

    vector<bool> can_reach = is_possible;
    // rapidly decrease chances
    double p = 1;
    for (auto &x : possible) {
        if (randomDouble(0.0, 1.0) > p)
            is_possible[x] = false;
        p *= 0.01;
    }
    if (randomElement({true, false})) {
        cerr << "meant to be no\n";
        is_possible = vector<bool>(COMPONENTS, false);
    }

    index = 0;
    vector< vector<int> > final_edges(N, vector<int>(SIGMA, -1));
    vector< vector<int> > component_chars(COMPONENTS);
    vector<int> component_begin(COMPONENTS, 0);
    vector<int> component_end(COMPONENTS, 0);
    for (int i = 0; i < COMPONENTS; ++i) {
        auto G = randomStronglyConnectedComponent(sizes[i]);
        vector< pair<int, int> > edges;
        for (int j = 0; j < sizes[i]; ++j)
            for (auto &next : G[j])
                edges.emplace_back(j, next);
        int need = MINEDGES;
        if (is_possible[i] || !can_reach[i])
            need = SIGMA;
        while (int(edges.size()) < need) {
            int from = randomInt(0, sizes[i] - 1);
            if (int(G[from].size()) == need)
                continue;
            int to = randomInt(0, sizes[i] - 1);
            G[from].push_back(to);
            edges.emplace_back(from, to);
        }

        vector<int> chars(SIGMA);
        for (int i = 0; i < SIGMA; ++i)
            chars[i] = i;
        random_shuffle(chars.begin(), chars.end());
        for (int i = 0; i < need; ++i)
            final_edges[edges[i].first + index][chars[i]] = edges[i].second + index;

        vector< vector<int> > picks(sizes[i]);
        for (int j = 0; j < sizes[i]; ++j)
            for (int k = 0; k < need; ++k)
                if (final_edges[j + index][chars[k]] == -1)
                    picks[j].emplace_back(chars[k]);
        for (int j = 0; j < sizes[i]; ++j)
            randomShuffle(picks[j].begin(), picks[j].end());
        for (int i = need; i < int(edges.size()); ++i) {
            auto how = picks[edges[i].first].back();
            final_edges[edges[i].first + index][how] = edges[i].second + index;
            picks[edges[i].first].pop_back();
        }

        chars.resize(need);
        component_chars[i] = chars;
        component_begin[i] = index;
        index += sizes[i];
        component_end[i] = index;
    }

    for (int i = 0; i < COMPONENTS; ++i) {
        vector< pair<int, int> > empty_edges;
        for (int j = component_begin[i]; j < component_end[i]; ++j)
            for (int k = 0; k < SIGMA; ++k)
                if (final_edges[j][k] == -1)
                    empty_edges.emplace_back(j, k);
        randomShuffle(empty_edges.begin(), empty_edges.end());
        for (int j = 0; j < int(empty_edges.size()) && j < int(component_edges[i].size()); ++j) {
            final_edges[empty_edges[j].first][empty_edges[j].second] = randomInt(component_begin[component_edges[i][j]], component_end[component_edges[i][j]] - 1);
        }
    }

    for (int i = 0; i < COMPONENTS; ++i) {
        vector< pair<int, int> > empty_edges;
        for (int j = component_begin[i]; j < component_end[i]; ++j)
            for (int k = 0; k < SIGMA; ++k)
                if (final_edges[j][k] == -1)
                    empty_edges.emplace_back(j, k);
        vector<bool> bad_sigma(SIGMA, true);
        for (auto &c : component_chars[i])
            bad_sigma[c] = false;

        for (auto &p : empty_edges) {
            if (bad_sigma[p.second] || randomElement({true, false})) { // well, this is that, can't do much
                int from = component_end[i];
                if (int(component_chars[i].size()) == SIGMA)
                    from = component_begin[i];
                if (i >= source_component && i <= source_component + length - 1)
                    from = component_begin[source_component + length];
                final_edges[p.first][p.second] = randomInt(from, index - 1);
            } else {
                final_edges[p.first][p.second] = randomInt(component_begin[i], component_end[i] - 1);
            }
        }
    }

    vector<int> final_nodes;
    for (int i = 0; i < COMPONENTS; ++i)
        if (has_final[i]) {
            auto aux = vector<int>();
            for (int j = component_begin[i]; j < component_end[i]; ++j)
                aux.push_back(j);
            aux = randomSubsequence(aux, Boolean::False);
            final_nodes.insert(final_nodes.end(), aux.begin(), aux.end());
        }
    cout << 1 << "\n";
    cout << N << " " << SIGMA << " " << final_nodes.size() << "\n";

    vector<int> map_nodes(N), remap_nodes(N);
    for (int i = 0; i < N; ++i)
        map_nodes[i] = i;
    randomShuffle(map_nodes.begin(), map_nodes.end());
    for (int i = 0; i < N; ++i)
        if (map_nodes[i] == 0) {
            int switch_with = randomInt(component_begin[source_component], component_end[source_component] - 1);
            swap(map_nodes[i], map_nodes[switch_with]);
        }

    for (int i = 0; i < N; ++i)
        remap_nodes[map_nodes[i]] = i;
    for (auto x : shuffle(final_nodes))
        cout << map_nodes[x] + 1 << " ";
    cout << "\n";
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < SIGMA; ++j) {
            cout << map_nodes[final_edges[remap_nodes[i]][j]] + 1 << " ";
        }
        cout << "\n";
    }
}
