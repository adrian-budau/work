#include <iostream>
#include <fstream>
#include <set>
#include <vector>

using namespace std;

struct piesa {
    int x, y;
};

struct calance {
    vector<piesa> H, V;
    piesa boss;
};

int main() {
    ifstream cin("afridge.in");
    ofstream cout("afridge.out");

    int N; cin >> N;
    
    calance X;
    cin >> X.boss.x >> X.boss.y;
