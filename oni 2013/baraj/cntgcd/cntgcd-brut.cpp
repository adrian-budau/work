#include <cstdio>
#include <algorithm>
#include <fstream>
#include <iostream>

using namespace std;

long long n , d;
int sol;
int main() {
    ifstream cin("cntgcd.in");
    ofstream cout("cntgcd.out");
    
   
    cin >> n >> d;

    for(int i = 1; i <= n; ++i)
    for(int j = i; j <= n; ++j)
        if( __gcd(i, j) == d) 
        ++sol;

    cout << sol << "\n";

    return 0;
}
