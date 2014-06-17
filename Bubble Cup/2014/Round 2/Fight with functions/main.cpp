#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <string>
#include <set>
#include <limits>
#include <cstring>
#include <cassert>

using namespace std;

static const int kMaxPrime = 100007;
static const int kBase = 10000;
static const int kBaseDigits = 4;
static const int kDigits = 405 / kBaseDigits;

vector<int> generatePrimes() {
    vector<int> sieve(kMaxPrime, 0);
    vector<int> result;
    for (int i = 2; i < kMaxPrime; ++i)
        if (sieve[i] == 0) {
            result.push_back(i);
            for (int j = i + i; j < kMaxPrime; j += i)
                sieve[j] = 1;
        }
    return result;
}

static const vector<int> kPrimes = generatePrimes();

class BigInt {
  public:
    BigInt(string S = "0") {
        m_count = 0;
        m_digits = new int[kDigits];
        memset(m_digits, 0, sizeof(int) * kDigits);
        reverse(S.begin(), S.end());

        for (int i = 0; i < int(S.size()); i += kBaseDigits) {
            int power = 1;
            int number = 0;
            for (int j = i; j < i + kBaseDigits && j < int(S.size()); ++j) {
                number = number + power * (S[j] - '0');
                power *= 10;
            }
            m_digits[m_count++] = number;
        }
    }

    BigInt(const BigInt& that) {
        m_count = that.m_count;
        m_digits = new int[kDigits];
        memcpy(m_digits, that.m_digits, sizeof(int) * kDigits);
    }

    BigInt& operator=(const BigInt& that) {
        delete[] m_digits;
        m_count = that.m_count;
        m_digits = new int[kDigits];
        memcpy(m_digits, that.m_digits, sizeof(int) * kDigits);
        return *this;
    }

    BigInt& operator*=(const int& value) {
        int remainder = 0, i;
        for (i = 0; i < m_count || remainder; ++i, remainder /= kBase)
            m_digits[i] = (remainder += m_digits[i] * value) % kBase;

        m_count = i;
        return *this;
    }

    map<int, int> factorize() {
        map<int, int> M;
        for (vector<int>::const_iterator it = kPrimes.begin(); it != kPrimes.end() && (
                    m_count > 3 || (1LL * kBase * kBase * m_digits[2] + kBase * m_digits[1] + m_digits[0]) >= 1LL * (*it) * (*it)); ++it) {
            int power = 0;
            while ((*this) % *it == 0) {
                *this /= *it;
                ++power;
            }

            if (power > 0)
                M[*it] = power;
        }

        if (m_digits[0] != 1 || m_count != 1) {
            if (m_count > 2)
                M[-1] = 1;
            else
                M[m_digits[1] * kBase + m_digits[0]] = 1;
        }

        return M;
    }

    BigInt& operator/=(int number) {
        int remainder = 0;
        for (int i = m_count - 1; i >= 0; --i, remainder %= number)
            m_digits[i] = (remainder = remainder * kBase + m_digits[i]) / number;
        for (; m_count > 1 && !m_digits[m_count - 1]; --m_count);
        return *this;
    }

    int operator%(int number) const {
        int remainder = 0;
        for (int i = m_count - 1; i >= 0; --i, remainder %= number)
            remainder = remainder * kBase + m_digits[i];
        return remainder;
    }

    ~BigInt() {
        delete[] m_digits;
    }

  private:
    friend ostream& operator<<(ostream&, const BigInt&);
    int m_count;
    int* m_digits;
};

istream& operator>>(istream& stream, BigInt &object) {
    string S; stream >> S;
    object = BigInt(S);
    return stream;
}

ostream& operator<<(ostream& stream, const BigInt& object) {
    stream << object.m_digits[object.m_count - 1];
    for (int i = object.m_count - 2; i >= 0; --i) {
        int number = object.m_digits[i];
        if (number < 1000)
            cout << "0";
        if (number < 100)
            cout << "0";
        if (number < 10)
            cout << "0";
        stream << number;
    }
    return stream;
}

int main() {
    ios::sync_with_stdio(false);
    int T; cin >> T;

    while (T--) {
        int N; cin >> N;

        vector< pair< map<int, int>, map<int, int> > > numbers;
        set<int> primes;
        set<int> known;
        for (int i = 0; i < N; ++i) {
            BigInt A, B; cin >> A >> B;
            numbers.push_back(make_pair(A.factorize(), B.factorize()));

            for (map<int, int>::iterator it = numbers.back().first.begin(); it != numbers.back().first.end(); ++it)
                known.insert(it -> first);

            for (map<int, int>::iterator it = numbers.back().second.begin(); it != numbers.back().second.end(); ++it)
                primes.insert(it -> first);

            //for (map<int, int>::iterator it = numbers.back().first.begin(); it != numbers.back().first.end(); ++it)
            //    cerr << it->first << "^" << it->second << " ";
            //cerr << " => ";
            //for (map<int, int>::iterator it = numbers.back().second.begin(); it != numbers.back().second.end(); ++it)
            //    cerr << it->first << "^" << it->second << " ";
            //cerr << "\n";
        }

        map<int, vector< pair<int, int> > > possible;
        bool bad = false;
        for (set<int>::iterator it = primes.begin(); it != primes.end(); ++it) {
            map<int, int> maybe;
            bool first = true;
            for (int i = 0; i < N; ++i) {
                if (!numbers[i].second.count(*it))
                    continue;
                int need = numbers[i].second[*it];

                map<int, int> now;
                for (map<int, int>::iterator jt = numbers[i].first.begin(); jt != numbers[i].first.end(); ++jt)
                    if (need % jt->second == 0)
                        now[jt->first] = need / jt->second;

                if (first) {
                    swap(maybe, now);
                    first = false;
                } else {
                    for (map<int, int>::iterator jt = maybe.begin(); jt != maybe.end();)
                        if (now[jt->first] != jt -> second) {
                            map<int, int>::iterator kt = jt;
                            ++kt;
                            maybe.erase(jt);
                            jt = kt;
                        } else
                            ++jt;
                }
            }

            for (int i = 0; i < N; ++i) {
                if (numbers[i].second.count(*it))
                    continue;

                for (map<int, int>::iterator jt = numbers[i].first.begin(); jt != numbers[i].first.end(); ++jt)
                    maybe.erase(jt -> first);
            }

            possible[*it] = vector< pair<int, int> >(maybe.begin(), maybe.end());
            if (maybe.size() == 0)
                bad = true;
        }

        assert(known.count(-1) == 0 && primes.count(-1) == 0);
        if (known.size())
            assert(*known.rbegin() < kMaxPrime);
        if (primes.size())
            assert(*primes.rbegin() < kMaxPrime);

        int Q; cin >> Q;
        for (int i = 0; i < Q; ++i) {
            BigInt A; cin >> A;
            if (bad) {
                cout << "NO\n";
                continue;
            }

            map<int, int> m = A.factorize();
            bool current_bad = false;
            for (map<int, int>::iterator it = m.begin(); it != m.end(); ++it)
                if (!known.count(it -> first))
                    current_bad = true;

            BigInt answer("1");
            for (set<int>::iterator it = primes.begin(); it != primes.end() && !current_bad; ++it) {
                int total = 0;
                int maximum = numeric_limits<int>::min();
                int minimum = numeric_limits<int>::max();
                vector< pair<int, int> > &v = possible[*it];

                for (vector< pair<int, int> >::iterator jt = v.begin(); jt != v.end(); ++jt)
                    if (m.count(jt -> first)) {
                        maximum = max(maximum, m[jt->first] * jt->second);
                        minimum = min(minimum, m[jt->first] * jt->second);
                        ++total;
                    }

                if (total == 0)
                    continue;

                if (total != int(v.size()) || maximum != minimum) {
                    current_bad = true;
                    break;
                }

                for (int i = 0; i < maximum; ++i)
                    answer *= *it;
            }

            if (current_bad)
                cout << "NO\n";
            else
                cout << "YES " << answer << "\n";
        }
    }
}

