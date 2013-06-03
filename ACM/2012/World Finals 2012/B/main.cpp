#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

double calc(vector<double> F, double where) {
    double answer = 0;
    for (int i = 0; i < int(F.size()); ++i)
        answer += pow(where, i) * F[i];
    return answer;
}

double calc(vector<double> F, double from, double to) {
    return calc(F, to) - calc(F, from);
}

int main() {
    int N;
    int test = 0;
    while (cin >> N) {
        ++test;
        vector<double> A(N + 1);
        for (int i = 0; i <= N; ++i)
            cin >> A[i];

        vector<double> B(2 * N + 3, 0);
        for (int i = 0; i <= N; ++i)
            for (int j = 0; j <= N; ++j)
                B[i + j] += A[i] * A[j];
        for (int i = 2 * N + 1; i > 0; --i) {
            B[i] = (B[i - 1]) / double(i);
        }

        double xlow, xhigh; cin >> xlow >> xhigh;
        double inc; cin >> inc;
        double pi = acos(-1);
        double volume = calc(B, xlow, xhigh) * pi;

        cout.setf(ios::fixed, ios::floatfield);
        cout.precision(2);
        cout << "Case " << test << ": " << volume << "\n";

        if (volume < inc) {
            cout << "insufficient volume\n";
            continue;
        }

        bool ok = false;
        for (int i = 1; i <= 8; ++i)
            if (i * inc <= volume) {
                double left = xlow;
                double right = xhigh;
                for (int j = 0; j < 50; ++j) {
                    double mid = (left + right) / 2;
                    double vol = calc(B, xlow, mid);
                    if (vol * pi < i * inc)
                        left = mid;
                    else
                        right = mid;
                }
                if (ok)
                    cout << " ";
                else
                    ok = true;
                cout << left  - xlow;
            }
        cout << "\n";
    }
}
