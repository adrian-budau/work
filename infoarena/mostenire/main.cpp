#include <iostream>
#include <fstream>
#include <cassert>
#include <algorithm>
#include <vector>

using namespace std;

int main() {
    ifstream cin("mostenire.in");
    ofstream cout("mostenire.out");

    int N; int64_t K;
    assert(cin >> N >> K);
    assert(1 <= N && N <= 1000 * 1000);
    assert(1 <= K && K <= 1000LL * 1000 * 1000 * 1000 * 1000 * 1000);

    vector<int> V(N);
    for (int i = 0; i < N; ++i) {
        assert(cin >> V[i]);
        assert(1 <= V[i] && V[i] <= 2000 * 1000 * 1000);
    }

    int firstBigger = N;
    for (int i = 0; i < N; ++i)
        if (V[i] > V[0]) {
            firstBigger = i;
            break;
        }
    vector<int> partialSums(N);

    int sum = 0;
    for (int i = 0; i < N; ++i) {
        assert(2000 * 1000 * 1000 - sum >= V[i]);
        sum += V[i];
        partialSums[i] = sum;
    }

    if (K == 0 || (V.front() == V.back() && K % sum == 0)) {
        cout << 0 << "\n";
        for (int i = 0; i < N; ++i)
            cout << K / N << " ";
        cout << "\n";
        return 0;
    }

    auto tooMuch = [&](int64_t D, int64_t S) {
        // Condition(may overflow): V.front() * (D + S) + D >= V.back() * S)
        // Rewritten:
        // - V.front() * (S + D) >= V.back() * S - D
        // - !(V.front() * (S + D) < V.back() * S - D)
        // - !(V.front() < (V.back() * S - D) / (S + D)
        if ((V.back() * S - D) > 0 &&
            V.front() < ((V.back() * S - D) / (S + D) + int((V.back() * S - D) % (S + D) > 0)))
            return true;

        int step;
        for (step = 1; step <= N; step <<= 1);
        int find;
        for (find = -1; step; step >>= 1)
            if (find + step < N)
                if (S * V[find + step] < S * V.back() - D)
                    find += step;

        int64_t have = K - S * sum;
        if (find >= 0) {
            // this would overflow: need = (S * V.back() - D) * (find + 1) - partialSums[find] * S;
            int64_t minus = partialSums[find] * S;
            int64_t need = S * V.back() - D;
            // Rewrite: need * (find + 1) - minus > have
            // need * (find + 1) > have + minus
            // need > (have + minus) / (find + 1)
            if (need > (have + minus) / (find + 1))
                return true;
        }
        return false;
    };

    auto notEnough = [&](int64_t D, int64_t S) {
        // Rewrite: (S + D) * sums < K
        // (S + D) < K / sums
        if ((S + D) < K / sum + int(K % sum > 0))
            return true;

        if (firstBigger == N)
            return false;

        int64_t T = K - sum * S;
        int64_t canLowest = V.front() * firstBigger;
        // Rewrite: canLowest * D > T
        // canLowest > T / D
        if (canLowest > T / D)
            return false;
        canLowest *= D;
        //int64_t canHighest = ((S + D) * V.front() + D) * (N - firstBigger) - S * (partialSums.back() - partialSums[firstBigger - 1]);
        //we write it like an A - B
        int64_t B = S * (partialSums.back() - partialSums[firstBigger - 1]);
        // so instead of A - B > T
        // we check A > T + B
        // where A = ((S + D) * V.front() + D) * (N - firstBigger)
        // so A = part * (N - firstBigger)
        // so part * (N - firstBigger) > T + B
        // so part > (T + B) / (N - firstBigger)
        int64_t part = (S + D) * V.front() + D;
        if (part > (T + B) / (N - firstBigger))
            return false;
        int canHighest = part * (N - firstBigger) - B;
        if (canLowest + canHighest < T)
            return true;
        return false;
    };

    auto check = [&](int64_t D, int64_t S) {
        if (tooMuch(D, S))
            return 1;

        if (notEnough(D, S))
            return -1;

        return 0;
    };

    auto binarySearchBeginning = [&](int64_t D, int64_t from, int64_t to) {
        to /= sum;

        int64_t step;
        for (step = 1; step <= to - from; step *= 2);
        int64_t S;
        for (S = from - 1; step; step /= 2)
            if (S + step <= to) {
                auto state = check(D, S + step);
                if (state == -1)
                    S += step;
                else if (state == 0)
                    return make_pair(true, S + step);
            }
        return make_pair(false, 0LL);
    };

    auto binarySearchDifference = [&](int64_t from, int64_t to) {
        int64_t step;
        for (step = 1; step <= to - from; step *= 2);
        int64_t D;
        for(D = from - 1; step; step /= 2)
            if (D + step <= to)
                if (binarySearchBeginning(D + step, 0, K).first == false)
                    D += step;
        return make_pair(D + 1, binarySearchBeginning(D + 1, 0, K).second);
    };

    pair<int64_t, int64_t> answer = binarySearchDifference(0, K);
    cout << answer.first << "\n";

    vector<int64_t> solution(N), left(N);
    int64_t T = K - answer.second * sum;
    for (int i = 0; i < N; ++i) {
        solution[i] = answer.second * V[i];
        left[i] = V[i] * answer.first;
        if (V[i] > T / answer.first)
            left[i] = T;
    }

    for (int i = 0; i < N; ++i)
        if (solution[i] + answer.first < solution.back()) {
            int64_t need = solution.back() - answer.first - solution[i];
            left[i] -= need;
            solution[i] += need;
            T -= need;
        }


    int64_t step;
    for (step = 1; step <= left[0]; step *= 2);
    int64_t first;
    for (first = -1; step; step /= 2)
        if (first + step <= left[0]) {
            int64_t smallest = first + step + solution[0];
            int64_t need = first + step;
            int64_t canPut = first + step;
            for (int i = 1; i < N; ++i) {
                if (solution[i] < smallest)
                    need += smallest - solution[i];
                if (smallest + answer.first > solution[i])
                    if (canPut < T)
                        canPut += min(left[i], smallest + answer.first - solution[i]);
            }

            if (need > T)
                continue;
            if (canPut < T)
                first += step;
        }

    first++;

    solution[0] += first;
    T -= first;
    for (int i = 1; i < N; ++i)
        if (solution[i] < solution[0]) {
            int64_t now = solution[0] - solution[i];
            T -= now;
            solution[i] += now;
            left[i] -= now;
        }
    for (int i = 1; i < N; ++i) {
        int64_t now = min(left[i], solution[0] + answer.first - solution[i]);
        now = min(now, T);
        solution[i] += now;
        T -= now;
    }

    for (auto &v : solution)
        cout << v << " ";
    cout << "\n";
}
