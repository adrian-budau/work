#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <cassert>

using namespace std;

static const int kModulo = 1000 * 1000 * 1000 + 9;

template<class T>
class SmallIndexMap {
  public:
    class TWrapper {
      public:
        operator T&() const {
            if (m_map.m_iteration[m_index] != m_map.m_current_iteration)
                m_map.m_data[m_index] = T();
            return m_map.m_data[m_index];
        }

        TWrapper& operator=(const T& that) {
            m_map.m_data[m_index] = that;
            m_map.m_iteration[m_index] = m_map.m_current_iteration;
            return *this;
        }

      private:
        TWrapper(size_t index, SmallIndexMap& map):
                m_index(index),
                m_map(map) {
        }

        size_t m_index;
        SmallIndexMap& m_map;
        friend SmallIndexMap;
    };

    SmallIndexMap(int size):
        m_current_iteration(0),
        m_data(size, T()),
        m_iteration(size, 0) {
    }

    TWrapper operator[](size_t index) {
        return TWrapper(index, *this);
    }

    void clear() {
        ++m_current_iteration;
    }

  private:
    int m_current_iteration;
    vector<T> m_data;
    vector<int> m_iteration;
    friend TWrapper;
};

int main() {
    ifstream cin("progresii3.in");
    ofstream cout("progresii3.out");

    int64_t V; int N, K; cin >> V >> N >> K;
    assert(1 <= V && V <= 1000LL * 1000LL * 1000LL * 1000LL);
    assert(2 <= N && N <= 5000);
    assert(1 <= K && K <= 50);

    vector<int64_t> A(K);
    for (int i = 0; i < K; ++i) {
        cin >> A[i];
        assert(1 <= A[i] && A[i] <= V);
    }

    assert(is_sorted(A.begin(), A.end()));
    assert(A.back() - A.front() <= 50 * 1000);

    int64_t P = V / (N - 1); P %= kModulo;
    int answer = ((V % kModulo) * P - (N - 1) * ((P * (P + 1) / 2) % kModulo)) % kModulo;

    SmallIndexMap<int64_t> last(A.back() - A.front());
    for (int ratio = 1; ratio <= A.back() - A.front() && ratio <= V / (N - 1); ++ratio) {
        for (auto &x : A) {
            int64_t leftMost = x - int64_t(N - 1) * ratio;
            int64_t rightMost = x + int64_t(N - 1) * ratio;

            if (leftMost < 1)
                leftMost += int64_t((-leftMost) / ratio + 1) * ratio;
            if (rightMost > V)
                rightMost -= int64_t((rightMost - V - 1) / ratio + 1) * ratio;

            rightMost -= int64_t(N - 1) * ratio;

            if (leftMost <= rightMost) {
                leftMost /= ratio; ++leftMost;
                rightMost /= ratio; ++rightMost;

                if (last[x % ratio] < leftMost) {
                    answer = (answer - (rightMost - leftMost + 1)) % kModulo;
                    last[x % ratio] = rightMost;
                } else {
                    answer = (answer - (rightMost - last[x % ratio])) % kModulo;
                    last[x % ratio] = rightMost;
                }
            }
        }
        last.clear();
    }

    for (auto &x : A)
        for (int j = 1; j <= N; ++j) {
            int64_t maxRatio = V / (N - 1);
            if (j > 1)
                maxRatio = min(maxRatio, (x - 1) / (j - 1));
            if (j < N)
                maxRatio = min(maxRatio, (V - x) / (N - j));
            if (maxRatio > A.back() - A.front())
                answer = (answer - (maxRatio - (A.back() - A.front()))) % kModulo;
        }

    if (answer < 0)
        answer += kModulo;

    cout << answer << "\n";
}
