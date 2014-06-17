#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <cstring>

using namespace std;

class BigInt {
  public:
    BigInt() {
        memset(data_, 0, sizeof(data_));
        size_ = 1;
    }

    BigInt(int value) {
        size_ = 0;
        memset(data_, 0, sizeof(data_));
        do {
            data_[size_++] = value % base;
            value /= base;
        } while (value > 0);
    }

    BigInt& operator+=(const BigInt& that) {
        int t = 0, i;
        for (i = 0; i < size_ or i < that.size_ or t; ++i) {
            t += data_[i] + that[i];
            data_[i] = t >= base ? t - base : t;
            t = t >= base ? 1 : 0;
        }
        size_ = i;

        return *this;
    }

    int& operator[](int index) {
        return data_[index];
    }

    const int& operator[](int index) const {
        return data_[index];
    }

    int size() const {
        return size_;
    }

    BigInt& operator--() {
        for (int i = 0; i < size_; ++i) {
            --data_[i];
            if (data_[i] >= 0)
                break;
            data_[i] += base;
        }

        while (size_ > 1 and data_[size_ - 1] == 0)
            --size_;
        return *this;
    }

  private:
    static const int base = 1000 * 1000 * 1000;
    int size_;
    int data_[100];
};

ostream& operator<<(ostream& output, const BigInt& that) {
    output << that[that.size() - 1];
    for (int i = that.size() - 2; i >= 0; --i) {
        string S = to_string(that[i]);
        for (int i = S.size(); i < 9; ++i)
            output << 0;
        output << that[i];
    }

    return output;
}

int main() {
    ifstream cin("mult.in");
    ofstream cout("mult.out");

    int N, K; cin >> N >> K;
    vector<BigInt> many(K);
    many[0] = 1;

    for (int i = 0; i < N; ++i) {
        int V; cin >> V;
        vector<BigInt> next(K, 0);
        for (int j = 0; j < K; ++j) {
            next[j] += many[j];
            next[(j * 10 + V) % K] += many[j];
        }

        swap(many, next);
    }

    --many[0];
    cout << many[0] << "\n";
}
