#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

using namespace std;

struct triplu {
    int first, second, indice;

    bool operator<(const triplu &that) const {
        if (first == that.first)
            return second > that.second;
        return first < that.first;
    }
};

int main() {
    ifstream cin("people.in");
    ofstream cout("people.out");

    int N; cin >> N;

    vector<triplu> A(N);
    for (int i = 0; i < N; ++i) {
        cin >> A[i].first >> A[i].second;
        A[i].indice = i + 1;
    }

    sort(A.begin(), A.end());

    vector<int> stack;
    vector<int> from(N, -1);
    for (int i = 0; i < N; ++i) {
        auto it = lower_bound(stack.begin(), stack.end(), i, [&](int x, int y) {
            return A[x].second < A[y].second;
        });

        if (it == stack.end()) {
            if (stack.size())
                from[i] = stack.back();
            else
                from[i] = -1;

            stack.push_back(i);
        } else {
            if (it != stack.begin())
                from[i] = *(it - 1);
            *it = i;
        }
    }

    cout << stack.size() << "\n";
    for (int i = stack.back(); i != -1; i = from[i])
        cout << A[i].indice << " ";
    cout << "\n";
}

