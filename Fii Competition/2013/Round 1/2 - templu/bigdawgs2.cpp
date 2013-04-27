#include <fstream>
#include <vector>
#include <iostream>

using namespace std;

const int kModulo = 666013;

class Combinations {
  public:
    Combinations(const int &size):
            data_(size + 1, 0),
            idata_(size + 1, 0) {

        data_[0] = 1;
        for (int i = 1; i <= size; ++i)
            data_[i] = (static_cast<long long>(data_[i - 1]) * i) % kModulo;
        
        idata_[size] = pow(data_[size], kModulo - 2);
        for (int i = size - 1; i >= 0; --i)
            idata_[i] = (static_cast<long long>(idata_[i + 1]) * (i + 1)) % kModulo;
    }

  int operator()(const int &row, const int &column) const {
      if (row < column)
          return 0;
      int answer = (static_cast<long long>(data_[row]) * idata_[column]) % kModulo;
      return (static_cast<long long>(answer) * idata_[row - column]) % kModulo;
  }

  private:
    int pow(int a, int b) {
        if (b == 0)
            return 1;
        if (b % 2)
            return (static_cast<long long>(a) * pow(a, b - 1)) % kModulo;
        int x = pow(a, b / 2);
        return (static_cast<long long>(x) * x) % kModulo;
    }

    vector<int> data_;
    vector<int> idata_;
};

class Fibonacci {
  public:
    Fibonacci(const int &size):
            data_(size + 1, 0) {
        data_[1] = 1;
        for (int i = 2; i <= size; ++i)
            data_[i] = (data_[i - 1] + data_[i - 2]) % kModulo;
    }

    int operator()(const int &element) const {
        return data_[element];
    }

  private:
    vector<int> data_;
};

int main() {
    ifstream cin("templu.in");
    ofstream cout("templu.out");

    int N, M; cin >> N >> M;

    if (N == 1 && M == 1) {
        cout << 1 << "\n";
        return 0;
    }

    Fibonacci F(N);

    if (M == N || M == 1) {
        cout << F(N) << "\n";
        return 0;
    }

    Combinations C(N);
    int answer = 0;
    for (int i = 2; i < N; ++i) {
        // from i + 1, 2 to get to N, M
        // that's combinations of N - i - 1 taken M - 2
        answer = (answer + 1LL * F(i) * C(N - i - 1, M - 2)) % kModulo;

        // the same from i + 1 i
        // that's combinations of N - i - 1 taken M - i
        if (M >= i)
            answer = (answer + 1LL * F(i) * C(N - i - 1, M - i)) % kModulo;
    }

    cout << answer << "\n";
}
