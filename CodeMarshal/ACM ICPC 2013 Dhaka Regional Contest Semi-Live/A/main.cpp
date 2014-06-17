#include <iostream>

using namespace std;

int main() {
    do {
        int T; cin >> T;
        if (T == 0)
            break;

        int biggestH = -1, answer = -1;
        for (int i = 0; i < T; ++i) {
            int L, W, H; cin >> L >> W >> H;
            if (H > biggestH) {
                biggestH = H;
                answer = L * W * H;
            } else if (H == biggestH)
                answer = max(answer, L * W * H);
        }
        cout << answer << "\n";
    } while (true);
}
