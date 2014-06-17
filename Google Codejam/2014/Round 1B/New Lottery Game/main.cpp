#include <iostream>
#include <vector>
#include <map>

using namespace std;

struct config {
    int64_t step, A, B, K;

    bool operator<(const config& that) const {
        if (step != that.step)
            return step < that.step;
        if (A != that.A)
            return A < that.A;
        if (B != that.B)
            return B < that.B;
        return K < that.K;
    }
};

map<config, int64_t> M;

int64_t trim(int64_t A, int64_t step) {
    if (A >= step)
        return step - 1;
    return A;
}

int64_t solve(int64_t step, int64_t A, int64_t B, int64_t K) {
    if (A < 0 || B < 0)
        return 0;

    if (step == 1) {
        int64_t answer = 0;
        if (K & step) {
            answer = (A + 1) * (B + 1);
        } else {
            answer = (A + 1) * (B + 1);
            if (A == 1 && B == 1)
                --answer;
        }

        return (M[config{step, A, B, K}] = answer);
    }

    if (M.find(config{step, A, B, K}) != M.end())
        return M[config{step, A, B, K}];

    int64_t answer = 0;
    if (step & K) {
        answer = (trim(A, step) + 1) * (trim(B, step) + 1);
        if (A >= step)
            answer += (A - step + 1) * (trim(B, step) + 1);
        if (B >= step)
            answer += (trim(A, step) + 1) * (B - step + 1);

        answer += solve(step / 2, A - step, B - step, K);
    } else {
        answer = solve(step / 2, trim(A, step), trim(B, step), K) +
                 solve(step / 2, trim(A, step), B - step, K) +
                 solve(step / 2, A - step, trim(B, step), K);
    }

    return (M[config{step, A, B, K}] = answer);
}

int main() {
    int T; cin >> T;
    for (int test = 1; test <= T; ++test) {
        int64_t A, B, K; cin >> A >> B >> K;
        --A; --B; --K;

        int64_t step;
        for (step = 1; step <= A || step <= B || step <= K; step <<= 1);

        cout << "Case #" << test << ": " << solve(step, A, B, K) << "\n";
    }
}
