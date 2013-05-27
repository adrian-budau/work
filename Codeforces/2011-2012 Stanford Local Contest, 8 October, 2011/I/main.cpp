#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <random>

using namespace std;

const int kCases = 25;

int main() {
    int N;

    long double pi = acos((long double)(-1));
    while (cin >> N) {
        if (N == 0)
            break;

        vector<string> V(N);
        for (int i = 0; i < N; ++i)
            cin >> V[i];
        for (int i = 0; i < kCases; ++i) {
            long double x = -pi + (2 * pi) / kCases * i;

            vector<long double> stack;
            for (auto &op : V) {
                if (op == "x")
                    stack.push_back(x);
                else if (op == "sin")
                    stack.back() = sin(stack.back());
                else if (op == "cos")
                    stack.back() = cos(stack.back());
                else if (op == "tan")
                    stack.back() = tan(stack.back());
                else if (op == "+") {
                    long double z = stack.back();
                    stack.pop_back();
                    stack.back() += z;
                } else if (op == "-") {
                    long double z = stack.back();
                    stack.pop_back();
                    stack.back() -= z;
                } else {
                    long double z = stack.back();
                    stack.pop_back();
                    stack.back() *= z;
                }
            }
            if (stack.back() < -1e-6 || stack.back() > 1e-6) {
                cout << "Not an identity\n";
                break;
            }
            if (i == kCases - 1)
                cout << "Identity\n";
        }
    }
}
