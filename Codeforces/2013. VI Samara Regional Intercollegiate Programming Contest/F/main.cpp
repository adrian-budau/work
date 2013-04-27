#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>

using namespace std;

int abs(int x) {
    if (x < 0)
        return -x;
    return x;
}

int main() {
    int N, M, K, T; cin >> N >> M >> K >> T;

    deque<int> people(N);
    for (int i = 0; i < N; ++i)
        cin >> people[i];

    deque< pair<int, int> > shelter(M);
    for (int i = 0; i < M; ++i) {
        cin >> shelter[i].first;
        shelter[i].second = K;
    }

    sort(people.begin(), people.end());
    sort(shelter.begin(), shelter.end());

    int answer = 0;
    for (auto &x: people) {
        while (shelter.size()) {
            if (shelter.front().second == 0)
                shelter.pop_front();
            else if (x - shelter.front().first > T)
                shelter.pop_front();
            else break;
        }

        if (not shelter.size())
            break;

        if (abs(x - shelter.front().first) <= T) {
            ++answer;
            shelter.front().second--;
        }
    }

    cout << answer << "\n";
}
