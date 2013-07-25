#include <iostream>
#include <algorithm>
#include <map>
#include <string>
#include <vector>

using namespace std;

pair<int, int> dfs(int node, vector< vector<int> > &edges, vector<int> &T) {
    if (T[node] == -1)
        return make_pair(0, node);

    pair<int, int> answer(T[node], node);
    T[node] = -1;
    for (vector<int>::iterator it = edges[node].begin(); it != edges[node].end(); ++it)
        answer = max(answer, dfs(*it, edges, T));

    return answer;
}

int main() {
    while (true) {
        int N, M; cin >> N >> M;
        if (N == 0 and M == 0)
            break;

        map<string, int> where;
        vector<string> names(N);
        vector<int> T(N);
        for (int i = 0; i < N; ++i) {
            cin >> names[i] >> T[i];
            where[names[i]] = i;
        }

        vector< vector<int> > edges(N);
        for (int i = 0; i < M; ++i) {
            string A, B; cin >> A >> B;
            int x = where[A], y = where[B];
            edges[x].push_back(y);
            edges[y].push_back(x);
        }


        vector<string> answer;
        for (int i = 0; i < N; ++i)
            if (T[i] != -1)
                answer.push_back(names[dfs(i, edges, T).second]);

        sort(answer.begin(), answer.end());
        for (vector<string>::iterator it = answer.begin(); it != answer.end(); ++it)
            cout << *it << "\n";
    }
}
