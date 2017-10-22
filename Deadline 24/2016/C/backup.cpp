#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

vector<int> best(vector< vector<int> > types_for_remedies) {
    int maximum_type = 0;
    for (auto &v : types_for_remedies)
        for (auto &type : v)
            maximum_type = max(maximum_type, type);

    vector<bool> chosen(maximum_type + 1, false);
    vector<bool> cured(types_for_remedies.size(), false);
    vector<int> answer;
    while (count(cured.begin(), cured.end(), false)) {
        vector<int> cures(maximum_type + 1, 0);
        for (int i = 0; i < int(types_for_remedies.size()); ++i)
            if (!cured[i])
                for (auto &type : types_for_remedies[i])
                    cures[type]++;
        int best = max_element(cures.begin(), cures.end()) - cures.begin();
        chosen[best] = true;
        answer.emplace_back(best);
        for (int i = 0; i < int(types_for_remedies.size()); ++i)
            if (!cured[i])
                for (auto &type : types_for_remedies[i])
                    if (type == best) {
                        cured[i] = true;
                        break;
                    }
    }
    return answer;
}

int main() {
    ios::sync_with_stdio(false);

    int M; cin >> M;
    vector<int> L(M);
    vector<int> tableType;
    for (int i = 0; i < M; ++i) {
        cin >> L[i];
        for (int j = 0; j < L[i]; ++j)
            tableType.emplace_back(i);
    }

    int K; cin >> K;
    vector<int> duration(K);
    vector< vector<int> > need(K);
    for (int i = 0; i < K; ++i) {
        string S; getline(cin, S);
        istringstream is(S);
        int index; is >> index; --index;
        is >> duration[index];
        int x;
        while (is >> x) {
            need[index].emplace_back(x - 1);
        }
    }
}
