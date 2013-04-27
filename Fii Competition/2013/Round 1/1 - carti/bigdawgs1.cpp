#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    ifstream cin("carti.in");
    ofstream cout("carti.out");

    int N; long long M;
    cin >> N >> M;

    int answer = 0;
    vector< pair<int, int> > D;
    for (int i = 2; i <= N; ++i)
        if (M % i == 0) {
            D.push_back(make_pair(i, 0));
            while (M % i == 0) {
                M /= i;
                D.back().second++;
                answer++;
            }
        }

    if (M != 1) {
        cout << "0\n";
        return 0;
    }

    cout << answer << "\n";
    for (int i = 0; i < int(D.size()); ++i)
        cout << D[i].first << " " << D[i].second << "\n";
}
