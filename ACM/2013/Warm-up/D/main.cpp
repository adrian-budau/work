#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

int form(int a, int b) {
    return a * a + b * b - a * b;
}

int puncte(int N) {
    return N * (N + 1) / 2;
}

int main() {
    ofstream cout("bun.cpp");    
    vector<int> A(101, 0);
    for (int i = 1; i <= 100; ++i) {
        for (int a = 0; a < i; ++a)
            for (int b = 0; b < i; ++b)
                for (int c = 0; c < i; ++c) {
                    int l1 = form(a, i - b);
                    int l2 = form(b, i - c);
                    int l3 = form(c, i - a);
                    if (l1 == l2  and l2 == l3)
                        ++A[i];
                }
    }

    vector<long long> answer(101, 0);
    for (int i = 1; i <= 100; ++i)
        for (int j = 1; j <= i; ++j)
            answer[i] += puncte(i - j + 1) * A[j];

    cout << "{0, 0";
    for (int i = 1; i <= 100; ++i)
        cout << ", " << answer[i];
    cout << "}\n";
}
