#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class FenwickTree {
  public:
    FenwickTree(int size, int modulo):
            size_(size),
            modulo_(modulo),
            data_(size + 1, 0) {
    }

    void add(int x, int y) {
        for (; x <= size_; x += (x & -x))
            data_[x] = (data_[x] + y) % modulo_;
    }

    int query(int x) const {
        int answer = 0;
        for (; x; x -= (x & -x))
            answer = (answer + data_[x]) % modulo_;
        return answer;
    }

  private:
    int size_, modulo_;
    vector<int> data_;
};

int main() {
    int N, K, Q; cin >> N >> K >> Q;

    vector<int> A(Q), X(Q);
    for (int i = 0; i < Q; ++i) {
        cin >> A[i] >> X[i];
    }

    vector<int> Z(N);
    for (int i = 0; i < N; ++i)
        cin >> Z[i];

    FenwickTree F(K + 1, N / K);

    reverse(A.begin(), A.end());
    reverse(X.begin(), X.end());

    int rotit = 0;
    auto permute = [&](int from, int to, int much) {
        if (from > to)
            return;
        from = (from + rotit) % K;
        to = (to + rotit) % K;
        if (from <= to) {
            F.add(from + 1, much);
            F.add(to + 2, -much);
        } else {
            F.add(1, much);
            F.add(to + 2, -much);

            F.add(from + 1, much);
            F.add(K + 1, -much);
        }
    };

    for (int i = 0; i < Q; ++i) {
        int Anow = A[i], Xnow = X[i];
        if (Anow == 1) {
            rotit = (rotit + Xnow) % K;
            if (rotit < 0)
                rotit += K;
            continue;
        }

        Xnow = (Xnow - 1) % N;
        if (Xnow < 0)
            Xnow += N;
        int group = Xnow / K;

        permute(0, Xnow % K, group + 1);
        permute(Xnow % K + 1, K - 1, group);
        rotit = (rotit + Xnow + 1) % K;
        if (rotit < 0)
            rotit += K;
    }

    vector<int> answer(N);
    for (int i = 0; i < K; ++i) {
        int x = (i + rotit) % K;
        ++x;

        int grupa = F.query(x);
        if (grupa < 0)
            grupa += N / K;
        for (int j = 0, startZ = grupa * K + x - 1, startanswer = i; j < (N / K); ++j, startZ = (startZ + K) % N, startanswer = (startanswer + K) % N)
            answer[startanswer] = Z[startZ];
    }

    for (int i = 0; i < N; ++i)
        cout << answer[i] << " ";
    cout << "\n";
}
