#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

vector<int> prefix(const string &subject) {
    vector<int> pi(subject.size(), 0);

    // Ignore the first char because from there we match the whole string
    size_t matchTo = 0;
    for (size_t i = 1; i < subject.size(); ++i) {
        while (matchTo > 0 && subject[i] != subject[matchTo])
            matchTo = pi[matchTo - 1];

        if (subject[i] == subject[matchTo])
            ++matchTo;

        pi[i] = matchTo;
    }

    return pi;
}

int kmp(const string &pattern, const string &subject) {
    vector<int> pi = prefix(pattern);

    size_t matchTo = 0;
    for (size_t i = 0; i < subject.size(); ++i) {
        while (matchTo > 0 && subject[i] != pattern[matchTo])
            matchTo = pi[matchTo - 1];

        if (subject[i] == pattern[matchTo])
            ++matchTo;

        if (matchTo == pattern.size())
            return i;
    }

    return -1;
}

int main() {
    ifstream cin("infinitepatternmatching.in");
    ofstream cout("infinitepatternmatching.out");

    string S; cin >> S;

    string text;
    for (int i = 1; i < 300000; ++i) {
        int j;
        for (j = 0; (1 << j) <= i; ++j);
        for (--j; j >= 0; --j)
            if ((1 << j) & i)
                text += '1';
            else
                text += '0';
    }

    cout << kmp(S, text) + 1 << "\n";
}
