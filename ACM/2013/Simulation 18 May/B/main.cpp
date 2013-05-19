#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

using namespace std;

int main() {
    ifstream cin("bibliophile.in");
    ofstream cout("bibliophile.out");

    int N, Y; cin >> N >> Y;

    vector< pair<int, int> > V(N);
    for (int i = 0; i < N; ++i) {
        cin >> V[i].first;
        V[i].second = i + 1;
    }

    vector<int> answer;
    int total = 0;
    for (int _ = 0; _ < 3000000 / N; _++) {
        random_shuffle(V.begin(), V.end());
    
        int sum = 0;
        vector<int> aux;
        int i;
        for (i = 0; i < N; ++i) {
            if (sum + V[i].first > Y)
                break;
            sum += V[i].first;
            aux.push_back(V[i].second);
        }
        vector<pair<int, int> > V2(V.begin() + i, V.end());
        sort(V2.begin(), V2.end());
        while (V2.size()) {
            if (V2.back().first + sum > Y)
                V2.pop_back();
            else {
                aux.push_back(V2.back().second);
                sum += V2.back().first;
                V2.pop_back();
            }
        }

        if (Y - sum < Y - total) {
            total = sum;
            answer = aux;
        }
    }

    sort(answer.begin(), answer.end());
    cout << answer.size() << "\n";
    for (int i = 0; i < int(answer.size()); ++i)
        cout << answer[i] << " ";
    cout << "\n";
}
