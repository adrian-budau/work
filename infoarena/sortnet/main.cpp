#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
 
using namespace std;
 
int main() {
    ifstream cin("sortnet.in");
    ofstream cout("sortnet.out");
 
    int N, M; cin >> N >> M;
 
    vector< vector< pair<int, int> > > A(M);
    for (int i = 0; i < M; ++i) 
        for (int j = 0; j < N / 2; ++j) {
            char chr;
            int x, y;
            cin >> chr >> x;
            cin >> chr >> y;
            cin >> chr;
 
            A[i].push_back(make_pair(x - 1, y - 1));
        }
 
    vector<bool> can(1 << N, false);
    for (int i = 0; i < (1 << N); ++i) {
        bool good = true;
        for (int j = 0; j < N / 2; ++j)
            if (i & (1 << A[0][j].first))
                if (!(i & (1 << A[0][j].second))) {
                    good = false;
                    break;
                }
         
        if (good == false)
            continue;
 
        int aux = i;
        for (int j = 1; j < M; ++j)
            for (int k = 0; k < N / 2; ++k)
                if (aux & (1 << A[j][k].first))
                    if (!(aux & (1 << A[j][k].second))) {
                        aux ^= (1 << A[j][k].first);
                        aux ^= (1 << A[j][k].second);
                    }
         
        for (int j = 1; j < N; ++j)
            if (!(aux & (1 << j)))
                if (aux & (1 << (j - 1)))
                    good = false;
        can[i] = good;
    }
 
    int answer = 0;
    for (int i = 0; i < (1 << N); ++i) {
        int aux = i;
        for (int j = 0; j < N / 2; ++j)
            if (aux & (1 << A[0][j].first))
                if (!(aux & (1 << A[0][j].second))) {
                    aux ^= (1 << A[0][j].first);
                    aux ^= (1 << A[0][j].second);
                }
        answer += int(can[aux]);
    }
 
    cout << answer;
}
