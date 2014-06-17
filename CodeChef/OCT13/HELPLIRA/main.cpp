#include <iostream>
#include <limits>

using namespace std;

int main() {
    int N; cin >> N;

    int smallest = numeric_limits<int>::max(),
        largest = numeric_limits<int>::min();
    int wheresmallest = -1, wherelargest = -1;
    for (int i = 0; i < N; ++i) {
        int x1, y1, x2, y2, x3, y3; 
        cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;
        int area = abs(x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2));
        if (area <= smallest) {
            smallest = area;
            wheresmallest = i;
        }
        if (area >= largest) {
            largest = area;
            wherelargest = i;
        }
    }

    cout << wheresmallest + 1 << " " << wherelargest + 1 << "\n";
}
