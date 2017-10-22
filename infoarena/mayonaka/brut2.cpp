#include <fstream>
#include <vector>
#include <iostream>

using namespace std;

int main() {
    
    ifstream cin("mayonaka.in");
    ofstream cout("mayonaka.out");

    int n, m; cin >> n >> m;
    
    vector<int> v(n, 0);

    int cutoff = 0;
    while(cutoff * cutoff < n)
        cutoff++;
    
    vector<vector<int>> mars(cutoff + 1, vector<int> (n, 0));

    for(int i = 0; i < m; ++i) {
        int x, y, step, w; cin >> x >> y >> step >> w;
        x--, y--;
        if(step > cutoff) {
            for(int j = x; j <= y; j += step)
                v[j] += w;
        } else {
            mars[step][x] += w;
            int firstBad = x + ((y - x) / step + 1) * step;
            if(firstBad < n)
                mars[step][firstBad] -= w;
        }
    }
    
    for(int j = 1; j <= cutoff; ++j)
        for(int i = 0; i < n; ++i) {
            if(i - j >= 0)
                mars[j][i] += mars[j][i - j];
            v[i] += mars[j][i];
        }
    
    for(auto temp : v)
        cout << temp << " ";
}
