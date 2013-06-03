#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

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

    int query(int where) {
        int answer = 0;
        for (; where; where -= (where & -where))
            answer += data_[where];
        return answer;
    }

  private:
    int size_;
    vector<int> data_;
};

class FactorKeeper {
  public:
    FactorKeeper(const int &modulo, const int &size):
            modulo_(modulo),
            size_(size),
            lowest_prime_(size, 0),
            modular_inverse_(size, 0),
            powers_(size),
            count_(size, 0),
            product_(1),
            divide_(1) {
        int PHI = phi(modulo);
        for (int i = 2; i < size_; ++i) {
            if (!lowest_prime_[i]) {
                lowest_prime_[i] = i;
                primes_.push_back(i);
            }

            for (int j = 0; j < int(primes_.size()) and primes_[j] <= lowest_prime_[i] and i * primes_[j] < size; ++j)
                lowest_prime_[i * primes_[j]] = primes_[j];
        }

        for (int i = 0; i < int(primes_.size()); ++i)
            if (modulo_ % primes_[i]) {
                modular_inverse_[primes_[i]] = pow(primes_[i], PHI - 1);
            } else {
                int x = primes_[i];
                vector<int> &now = powers_[primes_[i]];
                now.reserve(size + 1);
                now.push_back(1);
                for (int j = 0; j < size; ++j)
                    now.push_back((1LL * now.back() * x) % modulo_);
            }

    }

    void add(int value, int many) {
        for (; lowest_prime_[value]; value /= lowest_prime_[value]) {
            int x = lowest_prime_[value];
            if (many == 1) { // we add
                if (count_[x] < 0)
                    divide_ /= x;
                else
                    if (modular_inverse_[x])
                        product_ = (static_cast<long long>(product_) * x) % modulo_;
                ++count_[x];
                continue;
            }

            if (count_[x] <= 0)
                divide_ *= x;
            else if (modular_inverse_[x])
                product_ = (static_cast<long long>(product_) * modular_inverse_[x]) % modulo_;
            --count_[x];
        }
    }

    int divide() const {
        return divide_;
    }

    int product() const {
        int answer = product_;
        for (vector<int>::const_iterator it = decompose_.begin(); it != decompose_.end(); ++it)
            if (count_[*it] > 0)
                answer = (static_cast<long long>(answer) * powers_[*it][count_[*it]]) % modulo_;
        return answer;
    }

  private:
    int pow(int x, int y) {
        if (y == 0)
            return 1;
        if (y % 2)
            return (1LL * pow(x, y - 1) * x) % modulo_;
        long long answer = pow(x, y / 2);
        return (answer * answer) % modulo_;
    }

    int phi(int V) {
        int answer = V;
        for (int i = 2; i * i <= V; ++i)
            if (V % i == 0) {
                if (i < size_)
                    decompose_.push_back(i);
                answer /= i;
                answer *= (i - 1);
                while (V % i == 0)
                    V /= i;
            }

        if (V > 1) {
            if (V < size_)
                decompose_.push_back(V);
            answer /= V;
            answer *= (V - 1);
        }

        return answer;
    }

    int modulo_, size_;
    vector<int> decompose_;
    vector<int> lowest_prime_, primes_;
    vector<int> modular_inverse_;
    vector< vector<int> > powers_;

    vector<int> count_;
    int product_;
    int divide_;
};

int main() {
    int N, M; cin >> N >> M;

    vector<int> V(N);
    for (int i = 0; i < N; ++i)
        cin >> V[i];

    int max_value = *max_element(V.begin(), V.end());
    vector<int> many(max_value + 1, 0);
    for (int i = 0; i < N; ++i)
        many[V[i]]++;

    FactorKeeper S(M, N + 1);
    for (int i = 1; i < N; ++i)
        S.add(i, 1);

    for (int i = 1; i <= max_value; ++i)
        for (int k = 1; k <= many[i]; ++k)
            S.add(k, -1);

    FenwickTree T(max_value);
    for (int i = 1; i <= max_value; ++i)
        if (many[i])
            T.add(i, many[i]);

    int answer = 0;
    for (int i = 0; i < N; ++i) {
        answer = (answer + 1LL * T.query(V[i] - 1) / S.divide() * S.product()) % M;
        S.add(many[V[i]], 1);
        --many[V[i]];
        T.add(V[i], -1);
        S.add(N - i - 1, -1);
    }

    cout << (answer + 1) % M << "\n";
}
