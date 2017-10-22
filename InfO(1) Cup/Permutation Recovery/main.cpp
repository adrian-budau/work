#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <tuple>

using namespace std;

class BigInt {
  static const int kBase = 1000 * 1000 * 1000;
  public:
    BigInt(const string& X) {
        m_data.reserve((int(X.size()) - 1) / 9 + 1);
        for (int i = int(X.size()) - 1; i >= 0; i -= 9) {
            int digit = 0;
            for (int j = max(i - 9 + 1, 0); j <= i; ++j)
                digit = digit * 10 + X[j] - '0';
            m_data.push_back(digit);
        }
    }

    void fit() {
        vector<int>(m_data).swap(m_data);
    }

    BigInt(int number) {
        if (number > 0)
            m_data.push_back(number);
    }

    BigInt& operator-=(const BigInt& that) {
        int rest = 0;
        for (int i = 0; i < int(m_data.size()); ++i) {
            m_data[i] -= rest;
            if (i < int(that.m_data.size()))
                m_data[i] -= that.m_data[i];
            if (m_data[i] < 0) {
                m_data[i] += kBase;
                rest = 1;
            } else {
                rest = 0;
            }
        }
        while (m_data.size() > 1 && m_data.back() == 0)
            m_data.pop_back();
        fit();
        return *this;
    }

    BigInt& operator+=(const BigInt& that) {
        int rest = 0;
        int i;
        for (i = 0; i < int(m_data.size()) || rest || i < int(that.m_data.size()); ++i, rest /= kBase) {
            if (i < int(m_data.size()))
                rest += m_data[i];
            if (i < int(that.m_data.size()))
                rest += that.m_data[i];
            if (i < int(m_data.size()))
                m_data[i] = rest % kBase;
            else
                m_data.push_back(rest % kBase);
        }
        fit();
        return *this;
    }

    bool operator<(const BigInt& that) const {
        if (zero())
            return false;
        if (that.zero())
            return true;
        if (m_data.size() != that.m_data.size())
            return m_data.size() < that.m_data.size();
        for (int i = m_data.size() - 1; i >= 0; --i)
            if (m_data[i] != that.m_data[i])
                return m_data[i] < that.m_data[i];
        return false;
    }

    bool zero() const {
        return m_data.size() == 0;
    }

  private:
    vector<int> m_data;
};

class SegmentTree {
  public:
    SegmentTree(int size):
        m_size(size),
        m_sub(m_size * 4, BigInt(0)),
        m_min(m_size * 4, BigInt(0)) {}

    void update(int pos, const BigInt& that) {
        update(1, 0, m_size, pos, that);
    }

    void sub(int from, int to, const BigInt& that) {
        sub(1, 0, m_size, from, to, that);
    }

    pair<BigInt, int> query_min() const {
        BigInt the_min = m_min[1];
        int node = 1;
        int begin = 0, end = m_size;
        while (end - begin > 1) {
            int mid = (begin + end) / 2;
            if (m_min[node * 2] < m_min[node * 2 + 1]) {
                end = mid;
                node = node * 2;
            } else {
                node = node * 2 + 1;
                begin = mid;
            }
        }
        return make_pair(the_min, begin);
    }

  private:
    void update(int node, int begin, int end, int pos, const BigInt& v) {
        if (end - begin == 1) {
            m_min[node] = v;
            return;
        }

        int mid = (begin + end) / 2;
        if (pos < mid)
            update(node * 2, begin, mid, pos, v);
        else
            update(node * 2 + 1, mid, end, pos, v);
        update_min(node);
    }

    void sub(int node, int begin, int end, int from, int to, const BigInt& v) {
        if (from <= begin && end <= to) {
            m_sub[node] += v;
            if (not m_min[node].zero())
                m_min[node] -= v;
            return;
        }

        int mid = (begin + end) / 2;
        if (from < mid)
            sub(node * 2, begin, mid, from, to, v);
        if (mid < to)
            sub(node * 2 + 1, mid, end, from, to, v);
        update_min(node);
    }

    void update_min(int node) {
        m_min[node] = m_min[node * 2];
        if (m_min[node * 2 + 1] < m_min[node])
            m_min[node] = m_min[node * 2 + 1];
        if (not m_min[node].zero())
            m_min[node] -= m_sub[node];
    }

    int m_size;

    vector<BigInt> m_sub, m_min;
};

int main() {
    ifstream cin("input");
    ofstream cout("output");

    int N; cin >> N;

    vector<BigInt> original(N, BigInt(0));
    for (int i = 0; i < N; ++i) {
        string number; cin >> number;
        original[i] = BigInt(number);
    }


    SegmentTree S(N);
    for (int i = N - 1; i >= 0; --i) {
        if (i > 0)
            original[i] -= original[i - 1];
        S.update(i, original[i]);
    }

    vector<int> answer(N);
    for (int i = 0; i < N; ++i) {
        BigInt minimum(0); int where;
        tie(minimum, where) = S.query_min();
        answer[where] = i + 1;
        S.update(where, BigInt(0));
        S.sub(where, N, original[where]);
        original[where] = BigInt(0);
    }

    for (int i = 0; i < N; ++i)
        cout << answer[i] << " ";
    cout << "\n";
}
