#include <iostream>
#include <string>
#include <vector>

using namespace std;

const int kModulo = 3046201;

class FenwickTree {
  public:
    FenwickTree(const int &size):
            size_(size),
            data_(size + 1, 0) {
    }

    void add(int where, int value) {
        for (; where <= size_; where += (where & -where))
            data_[where] += value;
    }

    int sum(int to) const {
        int answer = 0;
        for (; to > 0; to -= (to & -to))
            answer += data_[to];
        return answer;
    }

    int sum(int from, int to) const {
        return sum(to) - sum(from - 1);
    }

  private:
    int size_;
    vector<int> data_;
};

int pow(int x, int y) {
    if (y == 0)
        return 1;
    if (y % 2)
        return (1LL * x * pow(x, y - 1)) % kModulo;

    int z = pow(x, y / 2);
    return (1LL * z * z) % kModulo;
}

int main() {
    int N; cin >> N;

    vector<int> V(N);
    FenwickTree T(N);
    for (int i = 0; i < N; ++i) {
        cin >> V[i];
        T.add(i + 1, V[i]);
    }

    vector<int> fact(N * 30 + 1, 1), ifact(N * 30 + 1, 1);
    for (int i = 2; i <= N * 30; ++i)
        fact[i] = (1LL * fact[i - 1] * i) % kModulo;
    ifact[N * 30] = pow(fact[N * 30], kModulo - 2);
    for (int i = N * 30 - 1; i > 0; --i)
        ifact[i] = (1LL * ifact[i + 1] * (i + 1)) % kModulo;

    int Q; cin >> Q;
    for (int i = 0; i < Q; ++i) {
        string type; cin >> type;
        int x, y; cin >> x >> y;

        if (type == "change") {
            --x;
            T.add(x + 1, y - V[x]);
            V[x] = y;
        } else {
            int now = T.sum(x, y);

            int rest = now % (y - x + 1);
            int total = y - x + 1;
            int answer = fact[total];
            answer = (1LL * answer * ifact[rest]) % kModulo;
            answer = (1LL * answer * ifact[total - rest]) % kModulo;

            answer = (1LL * answer * fact[now]) % kModulo;
            answer = (1LL * answer * pow(ifact[now / total], total - rest)) % kModulo;
            answer = (1LL * answer * pow(ifact[now / total + 1], rest)) % kModulo;
            cout << answer << "\n";
        }
    }
}
