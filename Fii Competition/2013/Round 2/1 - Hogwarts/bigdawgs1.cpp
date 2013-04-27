#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

struct compare {
    compare(const vector<int> &data):
        data_(data) {
    }

    bool operator()(const int &x, const int &y) const {
        return data_[x] < data_[y];
    }

    const vector<int>& data_;
};

bool included(vector<int> a, vector<int> b) {
    sort(a.begin(), a.end());
    sort(b.begin(), b.end());
    vector<int> c(a.size());
    vector<int>::iterator it = set_intersection(a.begin(), a.end(), b.begin(), b.end(), c.begin());
    c.erase(it, c.end());
    return a == c;
}

int main() {
    ifstream cin("hogwarts.in");
    ofstream cout("hogwarts.out");

    int N, M, K; cin >> N >> M >> K;

    vector<int> L(M);
    for (int i = 0; i < M; ++i)
        cin >> L[i];

    vector<int> many(N + 1, 1);
    for (int i = 0; i < K; ++i) {
        int x; cin >> x;
        many[x] = 0;
    }

    int x; int y;
    while (cin >> x >> y) {
        many[x] += y;
    }

    int most = -1, where = -1;
    for (int i = 1; i <= N; ++i)
        if (many[i] > most) {
            most = many[i];
            where = i;
        }

    vector<int> poz(M);
    for (int i = 0; i < M; ++i)
        poz[i] = i;
    sort(poz.begin(), poz.end(), compare(L));

    vector< vector<int> > answer(M);
    for (int i = 0; i < min(M, most); ++i)
        answer[poz[i]].push_back(where);

    // let's put the others
    for (int i = 1; i <= N; ++i)
        if (many[i] > 0 && i != where)
            for (int j = M - 1; j >= 0; --j) {
                if (int(answer[poz[j]].size()) < L[poz[j]]) {
                    answer[poz[j]].push_back(i);
                    break;
                }

                if (answer[poz[j]][0] == where) {
                    answer[poz[j]][0] = i;
                    break;
                }
            }

    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < int(answer[i].size()); ++j)
            cout << answer[i][j] << " ";
        cout << "\n";
    }

    most = 0;
    for (int i = 0; i < M - 1; ++i)
        if (included(answer[poz[i]], answer[poz[i + 1]]))
            most = i + 1;

    cout << most + 1 << "\n";
    for (int i = 0; i <= most; ++i) {
        for (int j = 0; j < int(answer[poz[i]].size()); ++j)
            cout << answer[poz[i]][j] << " ";
        cout << "\n";
    }
}

