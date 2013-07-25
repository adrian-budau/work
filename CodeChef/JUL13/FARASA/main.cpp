#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <bitset>
#include <cstring>
#include <fstream>
#include <numeric>
#include <tr1/unordered_set>

using namespace std;
using namespace tr1;

const int kLimit = 10000;
const int kValue = 2000000000 / kLimit * 40;
const int kChangeToBruteForce = 5;
const int kRatio = 1000;
const int kPowerOf2 = 23;

const int kModulo = 897581057;
const int kModuloRoot = 45;
const int kPow = (1 << kPowerOf2);

bitset<kValue + 1> have;
int root_of[kPowerOf2 + 1];
int inverse_root_of[kPowerOf2 + 1];
int inverse_of[kPowerOf2 + 1];
int nlogn[kValue + 1];

int pow2(int x, int y) {
    if (y == 0)
        return 1;
    if (y % 2)
        return (1LL * pow2(x, y - 1) * x) % kModulo;
    int v = pow2(x, y / 2);
    return (1LL * v * v) % kModulo;
}

inline void next_step(int &a, int &b, int w) {
    int first = a;
    int second = (static_cast<long long>(b) * w) % kModulo;
    a = first + second;
    if (a >= kModulo)
        a -= kModulo;
    b = first - second;
    if (b < 0)
        b += kModulo;
}

class FFT {
  public:
    void clear(int limit) {
        for (size = 1, level = 0; size < limit; size <<= 1, ++level);
        root = root_of[level];
        inverse_root = inverse_root_of[level];

        memset(left, 0, sizeof(int) * size);
        memset(right, 0, sizeof(int) * size);
    }

    void setLeft(int x) {
        left[x] = 1;
    }

    void setRight(int x) {
        right[x] = 1;
    }

    void fft_multiply() {
        fft_transform(left, size, false);
        fft_transform(right, size, false);

        for (int i = 0; i < size; ++i)
            left[i] = (1LL * left[i] * right[i]) % kModulo;

        fft_transform(left, size, true);
    }

    bool exists(int x) {
        return left[x] > 0;
    }

  private:
    void fft_transform(int *a, int size, bool invert) {
        for (int i = 0; i < size; ++i) {
            int aux = reverse_bit(i);
            if (i < aux)
                swap(a[i], a[aux]); // so we don't need extra memory, we do everything on the spot
        }

        for (int length = 2, now = 1; length <= size; length <<= 1, ++now) {
            int current_root;
            if (not invert)
                current_root = root_of[now];
            else
                current_root = inverse_root_of[now];

            for (int i = 0; i < size; i += length) { // each group at a time
                int w = 1;
                for (int j = 0; j < length / 2; ++j) {
                    next_step(a[i + j], a[i + j + length / 2], w);
                    w = (static_cast<long long>(w) * current_root) % kModulo;
                }
            }
        }

        if (invert)
            for (int i = 0; i < size; ++i)
                a[i] = (static_cast<long long>(a[i]) * inverse_of[level]) % kModulo;
    }

    int reverse_bit(int x) {
        int answer = 0;
        for (int i = 0; i < level; ++i)
            if ((1 << i) & x)
                answer += (1 << (level - i - 1));
        return answer;
    }

    int size, level;
    int root, inverse_root;
    int left[kValue + 1], right[kValue + 1];
} F;

template<class Iterator>
void merge_fft(Iterator begin, Iterator end) {
    if (begin == end)
        return;
    if (end - begin + 1 <= kChangeToBruteForce or kRatio * nlogn[end - begin] > 1LL * (end - begin) * (end - begin)) {
        for (Iterator it = begin; it != end; ++it) {
            int sum = 0;
            for (Iterator jt = it; jt != end; ++jt) {
                sum += *jt;
                have[sum] = 1;
            }
        }
        return;
    }

    int total_sum = accumulate(begin, end, 0);
    Iterator middle;

    int sum = 0;
    for (Iterator it = begin; it != end; ++it) {
        sum += *it;
        if (sum > total_sum / 2) {
            middle = it;
            break;
        }
    }

    int left_sum = accumulate(begin, middle, 0);
    int right_sum = accumulate(middle + 1, end, 0);
    F.clear(left_sum + right_sum + 1);

    F.setLeft(0);
    for (Iterator it = begin; it != middle; ++it) {
        F.setLeft(left_sum);
        left_sum -= *it;
    }

    right_sum = 0;
    F.setRight(0);
    for (Iterator it = middle + 1; it != end; ++it) {
        right_sum += *it;
        F.setRight(right_sum);
    }

    F.fft_multiply();
    for (int i = 0; i <= total_sum - *middle; ++i)
        if (F.exists(i))
            have[*middle + i] = 1;
    merge_fft(begin, middle);
    merge_fft(middle + 1, end);
}

int main() {
    nlogn[1] = 1;
    for (int i = 2; i <= kValue; ++i)
        nlogn[i] = nlogn[i / 2] + 1;
    for (int i = 2; i <= kValue; ++i)
        nlogn[i] *= i;

#ifndef ONLINE_JUDGE
    ifstream cin("input");
    ofstream cout("output");
#endif
    root_of[kPowerOf2] = kModuloRoot;
    for (int i = kPowerOf2 - 1; i >= 0; --i)
        root_of[i] = (1LL * root_of[i + 1] * root_of[i + 1]) % kModulo;
    for (int i = kPowerOf2; i >= 0; --i)
        inverse_root_of[i] = pow2(root_of[i], kModulo - 2);


    for (int i = 0; i <= kPowerOf2; ++i)
        inverse_of[i] = pow2(1 << i, kModulo - 2);

    int N; cin >> N;
    vector<int> V(N);

    for (int i = 0; i < N; ++i)
        cin >> V[i];

    if (N <= kLimit) {
        unordered_set<long long> S;
        for (int i = 0; i < N; ++i) {
            long long sum = 0;
            for (int j = i; j < N; ++j) {
                sum += V[j];
                S.insert(sum);
            }
        }

        cout << S.size() - 1 << "\n";
        return 0;
    }

    merge_fft(V.begin(), V.end());

    cout << have.count() - 1 << "\n";
}
