#include <iostream>
#include <fstream>
#include <vector>
#include <set>

using namespace std;

int main() {
    ifstream cin("highcard.in");
    ofstream cout("highcard.out");

    int N; cin >> N;
    set<int> cards;
    for (int i = 1; i <= 2 * N; ++i)
        cards.emplace(i);

    vector<int> V(N);
    for (int i = 0; i < N; ++i) {
        cin >> V[i];
        cards.erase(V[i]);
    }

    int answer = 0;
    for (int i = 0; i < N; ++i) {
        auto it = cards.lower_bound(V[i]);
        if (it != cards.end()) {
            ++answer;
            cards.erase(it);
        } else
            cards.erase(cards.begin());
    }

    cout << answer << "\n";
}
