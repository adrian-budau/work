#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <map>
#include <algorithm>

using namespace std;

int main() {
    ios::sync_with_stdio(false);

    int C; cin >> C;
    map<string, array<string, 24> > which_room;
    map<string, int> have_hour;
    for (int i = 0; i < C; ++i) {
        string city_name; int rooms;
        cin >> city_name >> rooms;
        if (which_room.count(city_name) == 0) {
            fill(which_room[city_name].begin(), which_room[city_name].end(), "");
        }
        for (int i = 0; i < rooms; ++i) {
            string availability, room_name;
            cin >> availability >> room_name;
            int availability_mask = 0;
            for (int j = 0; j < 24; ++j)
                if (availability[j] == '.') {
                    availability_mask |= (1 << j);
                    which_room[city_name][j] = room_name;
                }
            have_hour[city_name] |= availability_mask;
        }
    }

    int Q; cin >> Q;
    while (Q--) {
        int many; cin >> many;
        vector<string> cities(many);
        int availability = (1 << 24) - 1;
        for (int i = 0; i < many; ++i) {
            cin >> cities[i];
            availability &= have_hour[cities[i]];
        }
        if (availability == 0) {
            cout << "No\n";
            continue;
        }
        cout << "Yes ";
        for (int i = 0; i < 24; ++i)
            if ((1 << i) & availability) {
                for (auto &city : cities)
                    cout << which_room[city][i] << " ";
                break;
            }
        cout << "\n";
    }
}
