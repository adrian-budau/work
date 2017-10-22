#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <limits>

using namespace std;

static const int kInfinite = 2000 * 1000 * 1000 + 7;

int playLarge(int, int, int, int, int);

int playSmall(int steps, int X, int Y, int smaller, int larger) {
   while (Y > 0 && smaller) {
        --smaller;
        if (steps % 2 == 0) {
            --Y;
            ++X;
        }
        ++steps;
    }

    if (Y > 0)
        return kInfinite;
    Y = -kInfinite;

    if (X <= 0)
        return steps;
    else
        return playLarge(steps, X, Y, smaller, larger);
}

int playLarge(int steps, int X, int Y, int smaller, int larger) {
    while (X > 0 && larger) {
        --larger;
        if (steps % 2) {
            --X;
            ++Y;
        }
        ++steps;
    }

    if (X > 0)
        return kInfinite;
    X = -kInfinite;

    if (Y <= 0)
        return steps;
    else
        return playSmall(steps, X, Y, smaller, larger);

}

int solve(int X, int Y, int smaller, int between, int larger) {
    int steps = 0;
    while ((X > 0 ||  Y > 0) && between) {
        if (steps % 2) {
            --X;
            --between;
        } else {
            --Y;
            --between;
        }
        ++steps;
    }

    if (X <= 0 && Y <= 0)
        return steps;

    if (X < 0)
        X = -kInfinite;
    if (Y < 0)
        Y = -kInfinite;

    // change the game, add then remove
    if (steps % 2)
        --X;
    else
        --Y;
    ++steps;

    return min(playSmall(steps, X, Y, smaller, larger), playLarge(steps, X, Y, smaller, larger));
}

int main() {
    ifstream cin("seriale.in");
    ofstream cout("seriale.out");

    int N, K; cin >> N >> K;
    vector<int> A(N), B(K);
    for (int i = 0; i < N; ++i)
        cin >> A[i];
    for (int i = 0; i < K; ++i)
        cin >> B[i];

    auto V = A;
    sort(V.begin(), V.end());
    sort(B.begin(), B.end());
    // In the end we would have eliminated X small numbers and Y large numbers
    // If we fix these, we know we can first eliminate everything between the Xth smalest and Yth largest
    int answer = K + 1;
    int Y = N;
    for (int X = 0; X < N; ++X)
        for (Y = min(Y, N - X - 1); Y >= 0; --Y) {
            int largestSmall = X > 0 ? V[X - 1] : -kInfinite;
            int smallestLarge = Y > 0 ? V[N - Y] : kInfinite;

            vector<int> left;
            for (auto &x : A)
                if (x > largestSmall && x < smallestLarge)
                    left.push_back(x);
            if (!is_sorted(left.begin(), left.end()))
                break;
            auto splitSmall = lower_bound(B.begin(), B.end(), largestSmall);
            auto splitLarge = lower_bound(B.begin(), B.end(), smallestLarge);

            int smaller = splitSmall - B.begin();
            int between = splitLarge - splitSmall;
            int larger = B.end() - splitLarge;

            answer = min(answer, solve(X, Y, smaller, between, larger));
        }

    if (answer > K)
        answer = -1;
    cout << answer << "\n";
}
