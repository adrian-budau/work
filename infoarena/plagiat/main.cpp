#include <iostream>
#include <unordered_map>
#include <fstream>
#include <vector>

using namespace std;

namespace std {
    template<>
    class hash< pair<int, int> > {
      public:
        size_t operator()(const pair<int, int> &obj) const {
            return hasher_(obj.first * (1LL << 32) + obj.second);
        }

      private:
        hash<int64_t> hasher_;
    };
}

bool writtenInTheStars(const vector< pair<int, int> > &points) {
    unordered_map< pair<int, int>, int> M;
    M.rehash(points.size() * points.size());
    for (auto first : points)
        for (auto second : points)
            if (first != second) {
                pair<int, int> diff(first.first - second.first, first.second - second.second);
                if (++M[diff] == 3)
                    return true;
            }
    return false;
}

int main() {
    ifstream cin("plagiat.in");
    ofstream cout("plagiat.out");

    int T; cin >> T;

    while (T--) {
        int N; cin >> N;
        vector< pair<int, int> > points(N);
        for (int i = 0; i < N; ++i)
            cin >> points[i].first >> points[i].second;

        if (writtenInTheStars(points))
            cout << "DA\n";
        else
            cout << "NU\n";
    }
}
