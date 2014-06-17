#include <iostream>

using namespace std;

int main() {
    int T; cin >> T;

    for (int test = 1; test <= T; ++test) {
        int N, K, C; cin >> N >> K >> C;

        int best = N;
        for (int zeroes = 0; zeroes < N; ++zeroes) {
            int now = zeroes;
            int first_type = K / (N - zeroes); // second type is first_type + 1
            int of_second_type = K % (N - zeroes);
            int of_first_type = (N - zeroes) - of_second_type;

            int need = C;
            need -= (N - zeroes) * first_type; // this much we can take from each of them without risking anything
            if (need > 0) // if we still need stuff he're screwed, we've got of_first_type empty jars which with bad luck ...
                now += of_first_type;

            best = min(best, now);
        }

        cout << "Case #" << test << ": " << best + C << "\n";
    }
}
