#include <iostream>
#include <map>
#include <fstream>
#include <vector>
 
using namespace std;
 
int main() {
    ifstream cin("caraibe.in");
    ofstream cout("caraibe.out");
 
    int N; cin >> N;
 
    vector<int> A(N - 2);
    for (int i = 0; i < N - 2; ++i)
        cin >> A[i];
 
    vector<int> each(1, 1);
    int from = 0;
    for (int i = N - 2; i > 0; --i) {
        int total = N - i - 1;
        while (from < int(each.size()) && total > A[i - 1])
            total -= each[from++];
 
        if (total < A[i - 1])
            each[--from] = A[i - 1] - total;
        each.push_back(N - i - A[i - 1]);
        //for (int j = 0; j < int(each.size()); ++j)
        //    cerr << "(" << each.size() - j - 1 << ", " << each[j] << ")   ";
        //cerr << "\n";
    }
 
    long long answer = 0;
    for (int i = from; i < int(each.size()); ++i)
        answer += (each.size() - 1 - i) * each[i];
 
    cout << 10000000000LL - answer << "\n";
}
