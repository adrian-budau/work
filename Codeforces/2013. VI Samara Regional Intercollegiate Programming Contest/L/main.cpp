#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

int main() {
    string A; cin >> A;
    string B = A;
    reverse(B.begin(), B.end());
    
    for (size_t i = 0; i < A.size() / 2; ++i)
        A[i] = B[i];
    cout << A << "\n";
}
