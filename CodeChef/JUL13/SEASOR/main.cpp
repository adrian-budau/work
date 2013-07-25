#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <map>
#include <fstream>
#include <set>
#include <cstdlib>
#include <ctime>
#include <cassert>

using namespace std;

#ifndef ONLINE_JUDGE
const int kBacktracking = 0;
#else
const int kBacktracking = 8;
#endif
const int kInfinite = 0x3f3f3f3f;

struct pair_comp {
    bool operator()(const pair<int, int> &a, const pair<int, int> &b) const {
        if (a.first != b.first)
            return a.first > b.first;
        return a.second < b.second;
    }
};

vector<int> answer;

template<class Iterator>
bool isSorted(Iterator begin, Iterator end) {
    if (begin == end)
        return true;
    for (Iterator it = begin + 1; it != end; ++it)
        if (*(it - 1) > *it)
            return false;
    return true;
}

void sort_K(vector<int> &V, int where, int K) {
    answer.push_back(where + 1);
    int N = min<int>(V.size(), where + K);
    sort(V.begin() + where, V.begin() + N);
}

void sort_01(vector<int> now, vector<int> &orig, int K, int offset) {
    set< pair<int, int>, greater< pair<int, int> > > S;

    int sum = 0, inversions = 0;
    for (int i = 0; i < K; ++i)
        if (now[i] == 0)
            inversions += sum;
        else
            ++sum;

    int N = now.size();
    vector<int> cost(now.size());
    for (int i = 0; i < N; ++i) {
        cost[i] = inversions;
        S.insert(make_pair(inversions, i));

        if (i + 1 == N)
            break;

        int length = min(N, i + K) - i;
        if (now[i] == 1)
            inversions -= (length - sum);

        sum -= now[i];
        if (i + K < N) {
            if (now[i + K] == 0)
                inversions += sum;
            else
                ++sum;
        }
    }

    // it's all prepared
    while (S.size()) {
        int win = S.begin() -> first;
        if (win == 0)
            break;
        int where = S.begin() -> second;
        S.erase(S.begin());

        sort_K(orig, where + offset, K);
        int right = min<int>(now.size(), where + K);
        sort(now.begin() + where, now.begin() + right);

        // now we must recalculate
        int start = max(where - K + 1, 0);
        sum = 0;
        inversions = 0;
        for (int i = start; i < start + K and i < int(now.size()); ++i)
            if (now[i] == 0)
                inversions += sum;
            else
                ++sum;

        for (int i = start; i < min<int>(where + K, now.size()); ++i) {
            S.erase(make_pair(cost[i], i));
            cost[i] = inversions;
            S.insert(make_pair(inversions, i));

            if (i + 1 == N)
                break;

            int length = min(N, i + K) - i;
            if (now[i] == 1)
                inversions -= (length - sum);

            sum -= now[i];
            if (i + K < N) {
                if (now[i + K] == 0)
                    inversions += sum;
                else
                    ++sum;
            }
        }
    }
}

void merge_sort(vector<int> &V, int begin, int end, int K) {
    if (isSorted(V.begin() + begin, V.begin() + end))
        return; // nothing to do

    vector<int> aux(V.begin() + begin, V.begin() + end);
    sort(aux.begin(), aux.end());
    int begin2 = 0;
    int end2 = end - begin;
    while (begin < end and aux[begin2] == V[begin]) {
        ++begin;
        ++begin2;
    }

    while (begin < end and aux[end2 - 1] == V[end - 1]) {
        --end;
        --end2;
    }

    if (begin == end)
        return;

    aux = vector<int>(aux.begin() + begin2, aux.begin() + end2);

    // now we have a much smaller thing to sort
    if (end - begin <= K) { // if we can sort in one step
        sort_K(V, begin, K);
        return;
    }

    // nothing worked, so we search for a partition
    vector<int> left_most(aux.size());
    vector<int> right_most(aux.size());
    vector<int> check(aux.size(), 0);

    for (int i = 0; i < end - begin; ++i) {
        if (i + 1 == end - begin)
            check[i] = 1;
        else if (aux[i] < aux[i + 1])
            check[i] = 1;
    }

    set <pair<int, int> > S;
    S.insert(make_pair(-1, -1));
    for (int i = begin; i < end; ++i)
        if (S.rbegin() -> first < V[i])
            S.insert(make_pair(V[i], i));

    for (int i = begin; i < end; ++i) {
        if (not check[i - begin])
            continue;

        if (S.rbegin() -> first <= aux[i - begin])
            left_most[i - begin] = V.size() + 1;
        else {
            set< pair<int, int> >::iterator it = S.lower_bound(make_pair(aux[i - begin] + 1, -1));
            left_most[i - begin] = it -> second;
        }
    }

    S.clear();
    S.insert(make_pair(kInfinite, -1));
    for (int i = end - 1; i >= begin; --i)
        if (S.begin() -> first > V[i])
            S.insert(make_pair(V[i], i));

    for (int i = end - 1; i >= begin; --i) {
        if (not check[i - begin])
            continue;

        set< pair<int, int> >::iterator it = S.lower_bound(make_pair(aux[i - begin] + 1, -1));
        --it;
        right_most[i - begin] = it -> second;
    }

    int best = -kInfinite;
    int split_value = -1;
    vector<int> where;
    for (int i = 0; i < end - begin; ++i)
        if (check[i]) {
            if (right_most[i] - left_most[i] > best) {
                best = right_most[i] - left_most[i];
                split_value = aux[i];
                where.clear();
                where.push_back(i);
            } else if (right_most[i] - left_most[i] == best)
                where.push_back(i);
        }

    int who = 0;//rand() % where.size();
    int Where = where[who] + begin;
    split_value = aux[where[who]];
    // found the split value, damn that was hard
    for (int i = begin; i < end; ++i)
        if (V[i] <= split_value)
            aux[i - begin] = 0;
        else
            aux[i - begin] = 1;

    // now sort only 0 and 1's
    // we keep for each group of K how good it is, as the number of values not at their position
    S.clear();

    sort_01(aux, V, K, begin);
    merge_sort(V, begin, Where + 1, K);
    merge_sort(V, Where + 1, end, K);
}

int main() {
    int seed = time(NULL);
    cerr << seed << "\n";
    srand(seed);
#ifndef ONLINE_JUDGE
    ifstream cin("input");
    ofstream cout("output");
#endif
    int T; cin >> T;

    while (T--) {
        int N, K; cin >> N >> K;
        if (N <= kBacktracking) { // we do backtracking
            vector<int> V(N);
            for (int i = 0; i < N; ++i)
                cin >> V[i];

            vector<int> F = V;
            sort(F.begin(), F.end());

            queue< vector<int> > Q;
            Q.push(V);

            map<vector<int>, vector<int> > M;
            M[V] = V;

            while (!Q.empty()) {
                if (M.count(F))
                    break;
                vector<int> now = Q.front();
                Q.pop();

                for (int i = 0; i < N; ++i) {
                    int right = min(N, i + K);
                    vector<int> aux = now;
                    sort(aux.begin() + i, aux.begin() + right);
                    if (M.insert(make_pair(aux, now)).second)
                        Q.push(aux);
                }
            }

            vector< vector<int> > steps;
            for (vector<int> now = F; now != V; now = M[now])
                steps.push_back(now);

            steps.push_back(V);
            reverse(steps.begin(), steps.end());
            cout << steps.size() - 1 << "\n";
            for (int i = 0; i < int(steps.size()) - 1; ++i) {
                vector<int> past = steps[i], future = steps[i + 1];
                for (int j = 0; j < N; ++j) {
                    int right = min(N, j + K);
                    vector<int> aux = past;
                    sort(aux.begin() + j, aux.begin() + right);
                    if (aux == future) {
                        cout << j + 1 << " ";
                        break;
                    }
                }
            }
            cout << "\n";
            continue;
        }

        vector<int> V(N);
        for (int i = 0; i < N; ++i)
            cin >> V[i];

        merge_sort(V, 0, N, K);

        cout << answer.size() << "\n";
        for (int i = 0; i < int(answer.size()); ++i)
            cout << answer[i] << " ";
        cout << "\n";

        assert(isSorted(V.begin(), V.end()));
        answer.clear();
    }
}
