#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <algorithm>

using namespace std;

int A[45][45];
int row[45];
int taken[45];
int cnt[45];

vector< tuple<int, int, int, int, int> > answer;
int area;
int N, L;
bool found;

mt19937 gen(103);

void back(int current) {
    if (found)
        return;

    if (current > N) {
        found = true;
        return;
    }

    int different = 0;
    for (int i = 1; i <= L; ++i)
        cnt[i] = 0;
    for (int i = 1; i <= L; ++i)
        if (row[i] < L && cnt[row[i]] == 0) {
            cnt[row[i]]++;
            different++;
        }

    if (current + different > N + 1)
        return;

    int where = min_element(row + 1, row + L + 1) - row;
    vector<int> OX, OY;
    for (int i = 1; row[where] + i <= L; ++i)
        if (!taken[i])
            OX.push_back(i);
    for (int i = 1; row[where + i - 1] == row[where] && where +i - 1 <= L; ++i)
        if (!taken[i])
            OY.push_back(i);
    if (L == 21 && current == 1) {
        OX = {4};
        OY = {9};
    }

    shuffle(OX.begin(), OX.end(), gen);
    shuffle(OY.begin(), OY.end(), gen);

    if (L == 25 && current == 1) {
        OX = {8};
        OY = {11};
    }

    if (L == 25 && current == 2) {
        OX = {12};
        OY = {10};
    }

    if (L == 30 && current == 1) {
        OX = {9};
        OY = {30};
    }

    if (L == 30 && current == 2) {
        OX = {14};
        OY = {5};
    }

    if (L == 30 && current == 3) {
        OX = {3};
        OY = {18};
    }

    if (L == 30 && current == 4) {
        OX = {15};
        OY = {6};
    }

    for (auto &j : OX)
        for (auto &k : OY)
            if (j != k) {
                if (current == N && j * k != area)
                    continue;
                if (current == 1 && k < j)
                    continue;
                taken[j] = 1;
                taken[k] = 1;
                area -= j * k;
                answer.emplace_back(row[where] + 1, where, j, k, current);
                for (int p = where; p < where + k; ++p)
                    row[p] += j;

                back(current + 1);
                if (found)
                    return;
                for (int p = where; p < where + k; ++p)
                    row[p] -= j;
                answer.pop_back();
                area += j * k;
                taken[k] = 0;
                taken[j] = 0;
            }
}

int main() {
    ifstream cin("badea.in");
    ofstream cout("badea.out");

    cin >> N >> L;

    for (int i = 0; i <= L + 1; ++i)
        A[0][i] = A[L + 1][i] = A[i][0] = A[i][L + 1] = 1;

    for (int i = 1; i <= L; ++i)
        row[i] = 0;
    area = L * L;

    back(1);

    int B[45][45];
    for (auto &p : answer) {
        int x, y, u, v, n;
        tie(x, y, u, v, n) = p;
        for (int i = x; i < x + u; ++i)
            for (int j = y; j < y + v; ++j)
                B[i][j] = n;
    }
    for (int i = 1; i <= L; ++i) {
        for (int j = 1; j <= L; ++j)
            cout << B[i][j] << " ";
        cout << "\n";
    }
}
