#include <iostream>
#include <algorithm>
#include <cmath>
#include <fstream>

using namespace std;

const long double eps = 1e-9;

int cmp(long double a, long double b) {
    if (a + eps < b)
        return -1;
    if (b + eps < a)
        return 1;
    return 0;
}

int main() {
    ifstream cin("circles.in");
    ofstream cout("circles.out");

    int N; cin >> N;

    vector<long double> radii;

    auto dis = [&](long double x1, long double y1, long double x2, long double y2) {
        return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
    };

    for (int i = 0; i < N; ++i) {
        int x1, y1, x2, y2, x3, y3; cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;

        long double a = dis(x1, y1, x2, y2);
        long double b = dis(x1, y1, x3, y3);
        long double c = dis(x2, y2, x3, y3);

        long double cos = b * b + c * c - a * a;
        cos /= 2;
        cos /= b * c;

        long double sin = sqrt(1 - cos * cos);

        radii.push_back(a / sin);
        cerr << radii[i] << " ";
    }

    sort(radii.begin(), radii.end());

    long double last = -50;
    int answer = 0;
    int now = 0;
    for (int i = 0; i < N; ++i) {
        if (cmp(radii[i], last) == 1) {
            now = 1;
            last = radii[i];
        } else
            ++now;
        answer = max(answer, now);
    }

    cout << answer << "\n";
}
