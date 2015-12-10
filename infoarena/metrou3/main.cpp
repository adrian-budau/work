#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

static const int kModulo = 1000 * 1000 * 1000 + 7;
static const int kInverse4 = (kModulo + 1) / 4;

struct ZP {
    ZP(const int64_t &x = 0):
            v(x % kModulo) {
        if (v < 0)
            v += kModulo;
    }

    ZP operator*(const ZP& that) const {
        return 1LL * v * that.v;
    }

    ZP& operator+=(const ZP& that) {
        v += that.v;
        if (v >= kModulo)
            v -= kModulo;
        return *this;
    }

    ZP operator+(const ZP& that) const {
        return v + that.v;
    }

    ZP& operator-=(const ZP& that) {
        v -= that.v;
        if (v < 0)
            v += kModulo;
        return *this;
    }

    ZP operator-(const ZP& that) const {
        return v - that.v;
    }

    int v;
};

ZP getDistances(const vector<int> &arr) {
    ZP sum = 0, answer = 0;
    for (int i = 0; i < int(arr.size()); ++i) {
        answer += ZP(arr[i]) * i - sum;
        sum += arr[i];
    }

    return answer * 2;
}

ZP solve(vector<int> where, int length, int meet) {
    // first lets split it in two parts, the right side, until they meet again

    vector<int> left, right, total;
    for (auto x : where) {
        x -= meet;
        if (x < 0)
            x += length;
        if (x <= length / 2)
            left.push_back(x);
        else
            right.push_back(length - x);
        total.push_back(x);
    }

    sort(left.begin(), left.end());
    sort(right.begin(), right.end());
    sort(total.begin(), total.end());

    ZP answer = 0;
    // we just need to add the distances between each of the left side, right side and both
    ZP cost = getDistances(total);
    // now let's add the waiting times
    for (int i = 0; i < int(total.size()); ++i) {
        cost += ZP(i) * (length - total[i]); // for the paths with subway B
        cost += ZP(total.size() - 1 - i) * total[i]; // for the paths with subway A
    }

    if (total[0] == 0) { // freaking special case, ah well so be it
        for (auto &v : total) {
            cost -= v;
            cost += min(v, length - v);
        }
    }

    int between = total.size();
    int outside = 0;

    int onLeft = left.size(), onRight = right.size();
    right.push_back(length / 2);
    int posLeft = 0, posRight = 0, N = left.size(), M = right.size();
    int now = 0;
    ZP sumLeft = 0, sumRight = -length / 2, sumLeftPast = 0, sumRightPast = 0;
    for (auto &v : left)
        sumLeft += v;
    for (auto &v : right)
        sumRight += v;
    while (posLeft < N || posRight < M) {
        int next = 0;
        if (posRight == M || (posLeft < N && left[posLeft] < right[posRight]))
            next = left[posLeft];
        else
            next = right[posRight];
        // all paths actually take 1 second less, because in this time we only wait for subways
        // so we need to add cost - i * (between + outside) * (between + outside - 1) for i from 1 to next - now
        // which is (next - now) * cost - (next - now) * (next - now + 1) / 2 * (between + outside) * (between + outside - 1)
        // which is (next - now) * (cost - (next - now + 1) * (between + outside) * (between + outside - 1) / 2)
        answer += ZP(next - now) * (cost - ZP(next - now + 1) * ZP(1LL * (between + outside) * (between + outside - 1) / 2));
        cost -= ZP(next - now) * (between + outside) * (between + outside - 1);

        // Now lets rmove this fellow
        if (posRight == M || (posLeft < N && left[posLeft] <= right[posRight])) {
            between--;
            outside++;
            onLeft--;
            sumLeft -= left[posLeft];
            // we can classify our problem in 3 big cases
            // 1: paths from x to y where x and y are between
            //      waiting time = 0, where are right there
            //      if y is our special, nothing changes, just ignore this
            //      if x is our node
            //          and y is left the distance just increases by length (we have to go round once)
            cost += 1LL * onLeft * length;
            //          and y is right we change the strategy, its better if we go round from B
            //            so we lose the onRight * length - sumRight
            //            and add sumRight + onRight * length
            //            but there is a damn special case with 0, he already does this
            if (left[posLeft] == 0) {
                cost += 1LL * onRight * length;
            } else {
                cost += sumRight * 2;
            }
            // 2: paths from x to y where x is between and y is outside
            //      our node cant be y, it was between until now
            //      if x is our node it's gonna get worse, instead of taking A, we're gonna take B sometimes
            //          if y is on the left we actually took B all the time, because it was smarter so skip this
            //          we still have to go from next on left to next on right so that doesn't change (the halfmoon part of the circle)
            //          so instead of adding the distances from next to all in past right, we'll add from left to all in past rights
            //          which means subtracting posRight * next - sumRightPast
            //          and adding posRight * next + sumRightPast
            cost += sumRightPast * 2;
            // 3: paths from x to y where x is outside and y is between
            //      our node cant be x, it was between until now
            //      if y is our node, well the distance was anyway exactly length, and now its gonna change to, guess what
            //          its gonna stay length
            sumLeftPast += left[posLeft];
            ++posLeft;
        } else { // just do the same
            between--;
            outside++;
            onRight--;
            sumRight -= right[posRight];
            cost += 1LL * onRight * length;
            cost += sumLeft * 2;
            cost += sumLeftPast * 2;

            sumRightPast += right[posRight];
            ++posRight;
        }

        // Hmm, that's weird, it feels very short, 70% of this source is comments :))
        // For whoever had the patiance to get here congratz
        now = next;
    }

    return answer;
}

int main() {
    ifstream cin("metrou3.in");
    ofstream cout("metrou3.out");

    int N, A, B; cin >> N >> A >> B;
    assert(3 <= N && N <= 100 * 1000);

    int T = 0;
    vector<int> D(N);
    for (int i = 0; i < N; ++i) {
        cin >> D[i];
        T += D[i];
        assert(1 <= D[i] && D[i] <= 200 * 1000 * 1000);
        assert(T <= 200 * 1000 * 1000);
    }

    assert(0 <= A && A < T);
    assert(0 <= B && B < T);

    vector<int> where(N);
    where[0] = 0;
    for (int i = 1; i < N; ++i)
        where[i] = where[i - 1] + D[i - 1];

    // double the distances, and times when A and B appear, now the subways meet exactly twice at integer locations
    T *= 2; A *= 2; B *= 2;
    for (auto &x : where)
        x *= 2;

    // let's see where they meet, there are exactly two places (at distance T / 2)
    int whereFirst = 0;
    if (A <= B)
        whereFirst = (B - A) / 2;
    else // pure symmetry
        whereFirst = T - (A - B) / 2;

    int whereSecond = whereFirst + T / 2;
    if (whereSecond >= T)
        whereSecond -= T;
    if (whereSecond < whereFirst)
        swap(whereFirst, whereSecond);

    cout << ((solve(where, T, whereFirst) + solve(where, T, whereSecond) - 1LL * T / 2 * N * (N - 1)) * kInverse4).v << "\n";
}

