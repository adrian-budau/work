#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>

using namespace std;

const int kModulo = 1000000000;

struct Matrix {
    Matrix(const int &_a = 0, const int &_b = 0, const int &_c = 0, const int &_d = 0):
            a(_a), b(_b), c(_c), d(_d) {}
    int a, b, c, d;

    Matrix operator*(const Matrix& that) const {
        return Matrix((1LL * a * that.a + 1LL * b * that.c) % kModulo, (1LL * a * that.b + 1LL * b * that.d) % kModulo,
                (1LL * c * that.a + 1LL * d * that.c) % kModulo, (1LL * c * that.b + 1LL * d * that.d) % kModulo);
    }
};

int main() {
    ifstream cin("calcul.in");
    ofstream cout("calcul.out");

    string A, B; cin >> A >> B;
    int C; cin >> C;

    int realA = 0;
    reverse(A.begin(), A.end());
    A.resize(min<int>(A.size(), C));
    reverse(A.begin(), A.end());
    for (int i = 0; i < C and i < int(A.size()); ++i)
        realA = realA * 10 + A[i] - '0';

    reverse(B.begin(), B.end());
    Matrix Z;
    Z.a = Z.b = realA;
    Z.d = 1;

    Matrix Answer;
    Answer.a = Answer.d = 1;

    for (int i = 0; i < int(B.size()); ++i) {
        int digit;
        if (B[i] >= '0' and B[i] <= '9')
            digit = B[i] - '0';
        else
            digit = 10 + B[i] - 'A';
        for (int j = 0; j < 4; ++j) {
            if ((1 << j) & digit)
                Answer = Answer * Z;
            Z = Z * Z;
        }
    }

    int modulo = 1;
    for (int i = 0; i < C; ++i)
        modulo *= 10;

    cout.fill('0');
    cout.width(C);
    cout << Answer.b % modulo << "\n";
}
