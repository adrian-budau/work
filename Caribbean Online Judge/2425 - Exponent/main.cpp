#include <iostream>
#include <cmath>

using namespace std;

int main() {
    while (true) {
        int N; cin >> N;

        if (N == 0)
            break;

        double best = 0;
        int bestX = 0, bestY = 0;

        for (int i = 0; i < N; ++i) {
            int X, Y; cin >> X >> Y;
            double aux = log(X) * Y;

            if (aux > best) {
                best = aux;
                bestX = X;
                bestY = Y;
            }
        }

        cout << bestX << " " << bestY << "\n";
    }
}
