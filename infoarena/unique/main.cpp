#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <stack>

using namespace std;

class DisjointSet {
  public:
    explicit DisjointSet(const int &size):
            group_(size) {
        for (int i = 0; i < size; ++i)
            group_[i] = i;
    }

    int group(int value) {
        if (group_[value] == value)
            return value;

        int answer = group(group_[value]);
        group_[value] = answer;
        return answer;
    }

    void unite(int first_value, int second_value) {
        group_[group(first_value)] = group(second_value);
    }


  private:
    vector<int> group_;
};

int main() {
    ifstream cin("unique.in");
    ofstream cout("unique.out");

    int tests; cin >> tests;
    for (int $ = 0; $ < tests; $++) {
        cerr << "Dollars are growing\n";
        int size; cin >> size;
        DisjointSet disjoint_set(size + 2); // for the infinite guards

        vector<int> values(size + 2, numeric_limits<int>::max());
        for (int i = 1; i <= size; ++i)
            cin >> values[i];

        values.back()--;  // so I don't have extra cases when 
                          // eliminating the last element

        stack<int> bigger_elements;    // we hold the positions in increasing order but decreasing in value
        bigger_elements.push(0);  // the element at 0 position
        vector<int> different_numbers(size + 2);  // the number of different numbers in each dijoint set
        vector<int> last_position(size + 1, -1);  // the last position a value occured
        int answer = 0;
        for (int i = 1; i <= size + 1; ++i) {
            int current_value = values[i];

            int current_different_numbers = 0;
            while (values[bigger_elements.top()] <= current_value) {
                int possible_max_position = bigger_elements.top();
                int possible_max_value = values[possible_max_position];

                bigger_elements.pop();

                current_different_numbers += different_numbers[disjoint_set.group(possible_max_position)];

                if (current_different_numbers == possible_max_value)
                    answer = max(answer, i - bigger_elements.top() - 1); // -1 because we don't take the current element

                disjoint_set.unite(possible_max_position, i);
            }

            bigger_elements.push(i);
            if (i > size)
                continue;
            different_numbers[disjoint_set.group(i)] = current_different_numbers + 1;

            if (last_position[current_value] > -1) {
                different_numbers[disjoint_set.group(last_position[current_value])]--;
            }

            last_position[current_value] = i;
        }

        cout << answer << "\n";
    }
}
