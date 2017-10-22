#include <iostream>
#include <fstream>
#include <vector>
#include <set>

using namespace std;

int main() {
    ifstream cin("cardgame.in");
    ofstream cout("cardgame.out");

    int N; cin >> N;
    set<int> cards;
    set<int> high_enemy, low_enemy;
    for (int i = 1; i <= 2 * N; ++i)
        cards.emplace(i);

    vector<int> V(N);
    for (int i = 0; i < N; ++i) {
        cin >> V[i];
        cards.erase(V[i]);
        if (i < N / 2)
            high_enemy.emplace(V[i]);
        else
            low_enemy.emplace(V[i]);
    }

    int answer = 0;
    for (int i = 2 * N; i >= 1; --i)
        if (cards.count(i)) {
            auto it = high_enemy.lower_bound(i);
            if (it != high_enemy.begin()) {
                --it;
                high_enemy.erase(it);
                cards.erase(i);
                ++answer;
            }
        }
    for (int i = 1; i <= 2 * N; ++i)
        if (cards.count(i)) {
            auto it = low_enemy.lower_bound(i);
            if (it != low_enemy.end()) {
                low_enemy.erase(it);
                cards.erase(i);
                ++answer;
            }
        }

    cout << answer << "\n";
}
