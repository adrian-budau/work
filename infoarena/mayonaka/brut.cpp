#include <vector>
#include <fstream>

using namespace std;

int main() {
    
    ifstream cin("mayonaka.in");
    ofstream cout("mayonaka.out");

    int n, m; cin >> n >> m;
    
    vector<long long> v(n, 0);

    for(int i = 0; i < m; ++i) {
        int x, y, step, w; cin >> x >> y >> step >> w;
        x--, y--;
        for(int j = x; j <= y; j += step)
            v[j] += w;
    }

    for(auto temp : v)
        cout << temp << " ";

    cout << "\n";
}
