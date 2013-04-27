#include <iostream>
#include <set>
#include <vector>
#include <algorithm>

using namespace std;

typedef pair<pair<int, int>, int> person;

int main() {
    int N; cin >> N;

    vector<int> V(N);
    for (int i = 0; i < N; ++i)
        cin >> V[i];

    vector<pair<pair<int, int>, int>> people(N);
    for (int i = 0; i < N; ++i) {
        cin >> people[i].first.first >> people[i].first.second;
        people[i].second = i;
    }

    sort(people.begin(), people.end(), [](const person &a, const person &b) {
        if (a.first.second == b.first.second)
            return make_pair(a.first.first, a.second) < make_pair(b.first.first, b.second);
        return a.first.second < b.first.second;
    });

    set< pair<int, int> > S;
    for (int i = 0; i < N; ++i)
        S.insert({V[i], i + 1});

    vector<int> answer(N);
    set< pair<int, int> > favorite;
    vector<person> who_takes_me(N + 1, {{-1, -1}, -1});
    vector<person> who_takes_me_favorite(N + 1, {{-1, -1}, -1});

    for (auto &guy: people) {
        auto it = S.lower_bound({guy.first.first, -1});
        if (it == S.end()) {
            cout << "Let's search for another office.\n";
            return 0;
        }

        if (it -> first > guy.first.second) {
            cout << "Let's search for another office.\n";
            return 0;
        }

        auto jt = favorite.begin();
        while ((jt = favorite.lower_bound({guy.first.first, -1})) != favorite.end()) {
            if (jt -> first > guy.first.second)
                break;
            if (*jt > *it)
                break;
            who_takes_me_favorite[jt -> second] = guy;
            favorite.erase(jt);
        }

        answer[guy.second] = it -> second;
        who_takes_me[it -> second] = guy;

        favorite.insert(*it);
        S.erase(it);
    }

    S.clear();
    for (int i = 0; i < N; ++i)
        S.insert({V[i], i + 1});

    vector<person> my_looser(N + 1, {{-1, -1}, -1});
    vector<int> my_room(N + 1, -1);

    for (auto &guy: people) {
        auto it = S.lower_bound({guy.first.first, -1}); ++it;
        if (it == S.end())
            continue;

        if (it -> first > guy.first.second) 
            continue;
        auto bitch = who_takes_me_favorite[answer[guy.second]];
        auto looser = who_takes_me[it -> second];

        if (bitch.second == -1 || looser.second == -1)
            continue;

        int freaky_room = V[answer[bitch.second] - 1];
        my_looser[guy.second] = looser;
        my_room[guy.second] = freaky_room;
    }

    vector<int> my_best(N + 1, numeric_limits<int>::min());
    for (int i = people.size() - 1; i >= 0; --i) {
        if (my_looser[people[i].second].second == -1)
            continue;
        if (my_room[people[i].second] == -1)
            continue;
        my_best[people[i].second] = my_best[my_looser[people[i].second].second];
        if (my_looser[people[i].second].first.second > my_best[people[i].second])
            my_best[people[i].second] = my_looser[people[i].second].first.second;

        if (my_best[people[i].second] >= my_room[people[i].second]) {
            cout << "Ask Shiftman for help.\n";
            return 0;
        }
    }
    cout << "Perfect!\n";
    for (auto &x: answer)
        cout << x << " ";
    cout << "\n";
}
