#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <limits>

using namespace std;

static const int64_t kInfinite = numeric_limits<int64_t>::max();

int64_t bitsTo(int64_t number) {
    int64_t answer = 0;
    int64_t bits;
    for (bits = 1; (1LL << bits) <= number; ++bits)
        answer += bits * ((1LL << bits) - (1LL << (bits - 1)));

    answer += bits * (number - (1LL << (bits - 1)) + 1);
    return answer;
}

int64_t fromString(const string& S) {
    int64_t v = 0;
    for (auto &c : S)
        v = v * 2 + c - '0';

    return v;
}

string toString(int64_t number) {
    string S;
    while (number) {
        S += number % 2 + '0';
        number /= 2;
    }
    reverse(S.begin(), S.end());
    return S;
}

int find(const string &A, const string& B) {
    size_t where = A.find(B);
    if (where == string::npos)
        return A.size();
    return where;
}

int64_t split2(string S) {
    if (S.size() < 2)
        return kInfinite;

    int N = S.size();

    int64_t answer = kInfinite;
    for (int L = N / 2; L <= N; ++L) {
        // Special case, first part is 111111...111
        {
            string aux = toString((1LL << L) - 1);
            aux += toString(1LL << L);
            int pos = find(aux, S);

            if (pos < L)
                answer = min(answer, bitsTo((1LL << L) - 1) + (pos + int(S.size()) - L));
        }
        for (int where = max(N - L, 1); where <= L; ++where)
            if (S[where] == '1') {
                int64_t part1 = fromString(S.substr(0, where));
                //part1 contains the last digits, if we add 1 we know that has to be the ending of real part 2
                int64_t endPart2 = part1 + 1;

                string endStringPart2 = toString(endPart2);
                // in case there was 1111..11, we already solve the case were the digits grow
                if (int(endStringPart2.size()) > where)
                    endStringPart2 = endStringPart2.substr(1);

                string realStringPart2 = S.substr(where);
                realStringPart2.resize(L, '0');
                for (int i = 0; i < int(endStringPart2.size()); ++i)
                    realStringPart2[realStringPart2.size() - endStringPart2.size() + i] = endStringPart2[i];

                int64_t realPart2 = fromString(realStringPart2);
                if (realPart2 <= 1 || realPart2 == (realPart2 & -realPart2))
                    continue;
                string aux = toString(realPart2 - 1);
                aux += realStringPart2;
                int pos = find(aux, S);
                if (pos < L)
                    answer = min(answer, bitsTo(realPart2 - 1) + (pos + int(S.size()) - L));
            }
    }

    return answer;
}

int64_t splitMore(string S) {
    int N = S.size();

    int64_t answer = kInfinite;
    for (int L = 1; L <= N - 2; ++L)
        for (int start = 0; start < L && start + L < N; ++start) {
            if (S[start] != '1')
                continue;
            int64_t number = fromString(S.substr(start, L));
            string aux;
            for (int64_t i = number; aux.size() <= S.size(); ++i)
                aux += toString(i);
            int decrease = 0;
            if (number > 1) {
                aux = toString(number - 1) + aux;
                decrease = toString(number - 1).size();
            }

            int pos = find(aux, S);
            if (pos < int(aux.size()))
                answer = min(answer, bitsTo(number - 1) + (pos + int(S.size()) - decrease));
        }

    return answer;
}

int main() {
    ifstream cin("infinitepatternmatching.in");
    ofstream cout("infinitepatternmatching.out");

    string S; cin >> S;
    int64_t pattern = fromString(S);
    if (pattern == 0) { // special case
        if (S.size() == 1)
            cout << 3 << "\n";
        else
            cout << bitsTo(1LL << int64_t(S.size())) << "\n";
        return 0;
    }

    int64_t answer = min(split2(S), splitMore(S));
    if (S[0] == '1')
        answer = min(answer, bitsTo(pattern));
    cout << answer << "\n";
}
