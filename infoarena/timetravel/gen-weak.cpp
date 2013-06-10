#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <inputGenerator.hpp>
#include <set>
#include <vector>
#include <cmath>

using namespace std;
using namespace inputGenerator;

template<class T>
T from_string(string S) {
    stringstream SS(S);
    T a; SS >> a;
    return a;
}

int main(int argv, char **argc) {
    int N = from_string<int>(argc[1]),
        M = from_string<int>(argc[2]),
        MAXV = from_string<int>(argc[3]),
        REFRESH = from_string<int>(argc[4]);          // a small values gives a lot of opeartions 1 and 3
    double KEEP = from_string<double>(argc[5]);       // what fraction of all numbers should on average have deletes
                                                      // if they are too many some deletes will dissapear
                                                      // if they have too few some will appear
    if (argv > 6) {
        reSeed(from_string<unsigned>(argc[6]));
    }

    vector<int> values = randomSample<int>(N, -MAXV, MAXV);

    map<int, vector<int> > T;
    vector<int> have_deletes, not_have_deletes;

    vector<int> have, not_have;
    for (int i = 0; i < N; ++i) {
        not_have.push_back(i);
        not_have_deletes.push_back(i);
    }

    string S = randomString(N, "1") + randomString(M, "0");
    S = shuffle(S);


    cout << N + M << "\n";
    for (int i = 0; i < N + M; ++i) {
        if ((S[i] == '1' or randomInt(1, REFRESH) == 1) and not_have.size()) {
            int position = randomUInt(0, not_have.size() - 1);
            int x = not_have[position];
            swap(not_have[position], not_have.back());
            not_have.pop_back();
            have.push_back(x);

            cout << "1 " << values[x] << "\n";
        } else {
            if (have.size() and randomInt(1, REFRESH) == 1) {
                int position = randomUInt(0, have.size() - 1);
                int x = have[position];
                swap(have[position], have.back());
                have.pop_back();
                not_have.push_back(x);

                cout << "3 " << values[x] << "\n";
                continue;
            }

            if ((randomInt(1, 2) == 2 and have.size()) or randomInt(1, 10) == 1) {
                cout << "5 " << randomInt(-MAXV, MAXV) << " " << randomInt(-MAXV, MAXV) << "\n";
                continue;
            }

            long long add = N, drop = N;
            int want = ceil(N * KEEP);
            if (int(have_deletes.size()) < want)
                add += 1LL * (want - have_deletes.size()) * (want - have_deletes.size());
            else
                drop += 1LL * (have_deletes.size() - want) * (have_deletes.size() - want);

            if (not have_deletes.size() or randomInt64(1, add + drop) <= add) {
                // If you refresh often we can add deletes for operations that do not exists
                if (randomInt(1, REFRESH) == 1 or int(T.size()) == N) {
                    int value = randomInt(0, N - 1);
                    int time = randomInt(-MAXV, MAXV);
                    T[value].push_back(time);
                    cout << "2 " << time << " " << values[value] << "\n";
                } else {
                    int value;
                    do {
                        int position = randomUInt(0, not_have_deletes.size() - 1);
                        value = not_have_deletes[position];
                        swap(not_have_deletes[position], not_have_deletes.back());
                    } while (T[value].size());

                    int time = randomInt(0, N - 1);
                    T[value].push_back(time);
                    cout << "2 " << time << " " << values[value] << "\n";
                }
            } else {
                // we want to erase something, we just chose a random number from the set and drop it
                int max_value = T.rbegin() -> first;
                int value = T.lower_bound(randomInt(0, max_value)) -> first;

                int position = randomUInt(0, T[value].size() - 1);
                int x = T[value][position];

                cout << "4 " << x << " " << values[value] << "\n";

                swap(T[value][position], T[value].back());
                T[value].pop_back();
                if (T[value].size() == 0) {
                    T.erase(value);
                    have_deletes.push_back(value);
                }

            }
        }
    }
}
