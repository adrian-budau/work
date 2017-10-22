#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

struct Operation {
    double cost;
    int pacient;
    int table;
};

int main(int argc, char **argv) {
    vector<string> arguments(argv + 1, argv + argc);
    double multA = 1.0, multB = 1.0;
    if (arguments.size() > 1) {
        multA = stod(arguments[0]);
        multB = stod(arguments[1]);
    }

    ios::sync_with_stdio(false);

    int M; cin >> M;
    vector<int> L(M);
    vector<int> tableType;
    vector<int> start(M), end(M);
    for (int i = 0; i < M; ++i) {
        cin >> L[i];
        start[i] = tableType.size();
        for (int j = 0; j < L[i]; ++j)
            tableType.emplace_back(i);
        end[i] = tableType.size();
    }
    int K; cin >> K;
    vector<int> duration(K);
    vector< vector<int> > need(K);
    cin.get();
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

    int N; cin >> N;
    int S = tableType.size();
    vector<int64_t> time_table(S, 0);

    int64_t T0 = 0;
    vector<int> identifier(N);
    vector< vector<int> > treatments(N);
    vector<int64_t> time_pacient(N, 0);
    cin.get();
    for (int i = 0; i < N; ++i) {
        string S; getline(cin, S);
        istringstream is(S);
        int index; is >> index; --index;
        int x;
        while (is >> x) {
            treatments[index].emplace_back(x - 1);
            T0 += duration[x - 1];
        }
        reverse(treatments[index].begin(), treatments[index].end());
        identifier[i] = i;
    }

    int64_t T = 0;
    int LS = 0;
    vector< vector< pair<int, int> > > answer(S);
    vector<int> count(N, 1);
    while (identifier.size()) {
        Operation best{10000000, -1, -1};
        for (int i = 0; i < int(identifier.size()); ++i) {
            int treatment = treatments[identifier[i]].back();
            for (auto &table : need[treatment]) {
                for (int j = start[table]; j < end[table]; ++j) {
                    int64_t best_end = max(time_pacient[identifier[i]], time_table[j]) + duration[treatment];
                    double cost = 0;
                    if (best_end > T) {
                        if (T == 0)
                            cost += multA * 20.0 / double(M) * double(T0) / double(best_end);
                        else
                            cost += multA * 20.0 / double(M) * double(T0) / double(T) - 20.0 / double(M) * double(T0) / double(best_end);
                    }
                    if (time_table[j] == 0) {
                        if (LS == 0)
                            cost += 0;
                        else
                            cost += multB * double(S) / double(LS) - double(S) / double(LS + 1);
                    }
                    if (cost < best.cost)
                        best = Operation{cost, i, j};
                    if (time_table[j] == 0)
                        break;
                }
            }
        }

        if (time_table[best.table] == 0)
            ++LS;
        int treatment = treatments[identifier[best.pacient]].back();
        int64_t treatment_end = max(time_pacient[identifier[best.pacient]], time_table[best.table]) + duration[treatment];
        T = max(T, treatment_end);
        answer[best.table].emplace_back(identifier[best.pacient], count[identifier[best.pacient]]++);

        time_pacient[identifier[best.pacient]] = treatment_end;
        time_table[best.table] = treatment_end;

        treatments[identifier[best.pacient]].pop_back();
        if (treatments[identifier[best.pacient]].size() == 0) {
            swap(identifier[best.pacient], identifier.back());
            identifier.pop_back();
        }
    }

    cout << LS << " " << T << "\n";
    for (int i = 0; i < S; ++i)
        if (answer[i].size()) {
            cout << i + 1 << " ";
            for (auto &p : answer[i]) {
                cout << p.first + 1 << " " << p.second << " ";
            }
            cout << "\n";
        }

    cout.flush();
    cerr.setf(ios::fixed, ios::floatfield);
    cout.precision(9);
    cerr << double(S) / double(LS) + 20.0 / double(M) * double(T0) / double(T) << "\n";
}
