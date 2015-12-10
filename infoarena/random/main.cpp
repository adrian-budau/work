#include <iostream>
#include <fstream>
#include <map>

using namespace std;

map<int, string> M = {
    {1982, "Robert Tarjan"},
    {1986, "Leslie Valiant"},
    {1990, "Alexander Razborov"},
    {1994, "Avi Wigderson"},
    {1998, "Peter Shor"},
    {2002, "Madhu Sudan"},
    {2006, "Jon Kleinberg"},
    {2010, "Daniel Spielman"},
    {2014, "Subhash Khot"}
};

int main() {
    ifstream cin("random.in");
    ofstream cout("random.out");

    int N; cin >> N;
    cout << M[N] << "\n";
}
