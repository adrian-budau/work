#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

const int kMaxValue = 1000 * 1000 * 1000;

class SegmentTree {
  public:
    SegmentTree(int size, int modulo):
            m_size(size),
            m_modulo(modulo) {
        m_data = new data[size * 4];
    }

    void set(int where, int value) {
        set(1, 0, m_size, where, value);
    }

    pair<int, int> maximum() const {
        return make_pair(m_data[1].maximum, m_data[1].where);
    }

    int query(int from, int to) {
        return query(1, 0, m_size, from, to);
    }

    void setStar(int where) {
        setStar(1, 0, m_size, where);
    }

    void multiply(int value) {
        m_data[1].maximum *= value;
        if (value == 0)
            m_data[1].count_good = m_size;
        m_data[1].product = (static_cast<long long>(m_data[1].product) * pow(value, m_data[1].count_good)) % m_modulo;
        m_data[1].lazy_multiply *= value;
    }

  private:
    struct data {
        data(int _product = 0, int _maximum = 0, int _lazy_multiply = 0):
                product(_product),
                maximum(_maximum),
                lazy_multiply(_lazy_multiply) {
        }

        int product;
        int maximum;
        int where;
        int count_good;
        int lazy_multiply;

        void combine(const data& left, const data& right, const int m_modulo) {
            product = static_cast<long long>(left.product) * right.product % m_modulo;
            if (left.maximum > right.maximum) {
                maximum = left.maximum;
                where = left.where;
            } else {
                maximum = right.maximum;
                where = right.where;
            }
            count_good = left.count_good + right.count_good;
            lazy_multiply = 1;
        }
    };

    void set(int node, int begin, int end, int where, int value) {
        if (end - begin == 1) {
            m_data[node].maximum = value;
            m_data[node].where = where;
            m_data[node].product = value % m_modulo;
            m_data[node].count_good = 1;
            m_data[node].lazy_multiply = 1;
            return;
        }

        push_lazy_info(node, begin, end);

        int mid = (begin + end) / 2;
        if (where < mid)
            set(node * 2, begin, mid, where, value);
        else
            set(node * 2 + 1, mid, end, where, value);

        m_data[node].combine(m_data[node * 2], m_data[node * 2 + 1], m_modulo);
    }

    int query(int node, int begin, int end, int from, int to) {
        if (from <= begin && end <= to)
            return m_data[node].product;

        push_lazy_info(node, begin, end);

        int mid = (begin + end) / 2;
        int answer = 1;
        if (from < mid)
            answer = (static_cast<long long>(answer) * query(node * 2, begin, mid, from, to)) % m_modulo;
        if (mid < to)
            answer = (static_cast<long long>(answer) * query(node * 2 + 1, mid, end, from, to)) % m_modulo;

        m_data[node].combine(m_data[node * 2], m_data[node * 2 + 1], m_modulo);
        return answer;
    }

    void setStar(int node, int begin, int end, int where) {
        if (end - begin == 1) {
            m_data[node].maximum = 0;
            m_data[node].where = where;
            m_data[node].product = 1;
            m_data[node].count_good = 0;
            m_data[node].lazy_multiply = 1;
            return;
        }

        push_lazy_info(node, begin, end);

        int mid = (begin + end) / 2;
        if (where < mid)
            setStar(node * 2, begin, mid, where);
        else
            setStar(node * 2 + 1, mid, end, where);

        m_data[node].combine(m_data[node * 2], m_data[node * 2 + 1], m_modulo);
    }

    int pow(int A, int B) {
        int result = 1;

        while (B) {
            if (B & 1)
                result = static_cast<long long>(A) * result % m_modulo;
            A = static_cast<long long>(A) * A % m_modulo;
            B /= 2;
        }

        return result;
    }

    void push_lazy_info(int node, int begin, int end) {
        if (m_data[node].lazy_multiply == 1)
            return;
        int lm = m_data[node].lazy_multiply;
        if (lm == 0) {
            int mid = (begin + end) / 2;
            m_data[node * 2].count_good = mid - begin;
            m_data[node * 2 + 1].count_good = end - mid;
        }

        m_data[node * 2].lazy_multiply *= lm;
        m_data[node * 2].maximum *= lm;
        m_data[node * 2].product = (static_cast<long long>(m_data[node * 2].product) * pow(lm, m_data[node * 2].count_good)) % m_modulo;
        m_data[node * 2 + 1].lazy_multiply *= lm;
        m_data[node * 2 + 1].maximum *= lm;
        m_data[node * 2 + 1].product = (static_cast<long long>(m_data[node * 2 + 1].product) * pow(lm, m_data[node * 2 + 1].count_good)) % m_modulo;
    }

    int m_size, m_modulo;
    data *m_data;
};

static const int kBufferSize = 32768;

class Reader {
  public:
    Reader(istream& stream):
            m_stream(stream) {
        m_position = kBufferSize - 1;
        m_buffer = new char[kBufferSize];
        nextChar();
    }

    Reader& operator>>(int &value) {
        value = 0;
        while (currentChar() < '0' || currentChar() > '9')
            nextChar();

        while (currentChar() >= '0' && currentChar() <= '9') {
            value = value * 10 + currentChar() - '0';
            nextChar();
        }

        return *this;
    }

    ~Reader() {
        delete[] m_buffer;
    }

  private:
    Reader(const Reader& that);

    void nextChar() {
        if (++m_position == kBufferSize) {
            m_position = 0;
            m_stream.read(m_buffer, kBufferSize);
        }
    }

    char currentChar() {
        return m_buffer[m_position];
    }

    istream& m_stream;
    int m_position;
    char *m_buffer;
};

int main() {
    ios::sync_with_stdio(false);
    Reader input(cin);
    int R, N, M; input >> R >> N >> M;

    assert(1 <= R && R <= 100000);
    assert(1 <= N && N <= 10000);
    assert(1 <= M && M <= 1000000000);
    SegmentTree T(N, M);
    for (int i = 0; i < N; ++i)
        T.set(i, 1);

    vector<int> position(2, 0);
    for (int i = 0; i < R; ++i) {
        int d, v, a, b; input >> d >> v >> a >> b;
        assert(0 <= d && d < N);
        assert(0 <= v && v <= 1000000000);
        assert(0 <= a && a <= b && b < N);
        ++b;
        T.set(d, v);

        int move = T.query(a, b);
        position[i % 2] = (position[i % 2] + move) % M;
        if (position[0] == position[1]) {
            while (true) {
                pair<int, int> maximum = T.maximum();
                if (static_cast<long long>(maximum.first) * position[0] > kMaxValue)
                    T.setStar(maximum.second);
                else
                    break;
            }
            T.multiply(position[0]);
        }

        cout << position[i % 2] << "\n";
    }
}

