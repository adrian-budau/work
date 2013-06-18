#include <iostream>
#include <set>
#include <limits>

using namespace std;

int main() {
    int N; cin >> N;

    multiset<int> left, right;

    auto median = [&]() -> int64_t {
        if (not left.size() and not right.size())
            return numeric_limits<int64_t>::min();
        if ((left.size() + right.size()) % 2) {
            return *left.rbegin() * 2LL;
        }

        return static_cast<int64_t>(*left.rbegin()) + *right.begin();
    };

    for (int i = 0; i < N; ++i) {
        char type; int value;
        cin >> type >> value;

        if (type == 'a') {
            if (2LL * value <= median())
                left.insert(value);
            else
                right.insert(value);
        } else {
            if (2LL * value <= median()) {
                if (left.find(value) == left.end()) {
                    cout << "Wrong!\n";
                    continue;
                }

                left.erase(left.find(value));
            } else {
                if (right.find(value) == right.end()) {
                    cout << "Wrong!\n";
                    continue;
                }

                right.erase(right.find(value));
            }
        }

        while (right.size() > left.size()) {
            left.insert(*right.begin());
            right.erase(right.begin());
        }

        while (left.size() > right.size() + 1) {
            right.insert(*left.rbegin());
            left.erase(left.find(*left.rbegin()));
        }


        int64_t answer = median();

        if (answer == numeric_limits<int64_t>::min()) {
            cout << "Wrong!\n";
            continue;
        }

        if (answer % 2) {
            if (answer == -1)
                cout << "-";
            cout << answer / 2 << ".5\n";
        } else
            cout << answer / 2 << "\n";
    }
}
