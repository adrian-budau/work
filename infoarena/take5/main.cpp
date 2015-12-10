#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

using namespace std;

static const int kMaxN = 1000;
int A[kMaxN];

int main() {
    ifstream cin("take5.in");
    ofstream cout("take5.out");

    int N, S; cin >> N >> S;

    for (int i = 0; i < N; ++i)
        cin >> A[i];
    sort(A, A + N);

    vector<int> sumFirst2, sumLast2;
    sumFirst2.reserve(N * N);
    sumLast2.reserve(N * N);
    sumFirst2.push_back(A[0] + A[1]);
    for (int i = 3; i < N; ++i)
        for (int j = 3; j < i; ++j)
            sumLast2.push_back(A[i] + A[j]);
    sort(sumLast2.begin(), sumLast2.end());

    int answer = 0;
    for (int middle = 2; middle + 2 < N; ++middle) {
        int need = S - A[middle];
        int U = sumFirst2.size();
        int V = sumLast2.size();

        int left = 0;
        int right = V - 1;
        for (; left < U && right >= 0;) {
            int newLeft, newRight;

            while (right >= 0 && sumFirst2[left] + sumLast2[right] > need)
                --right;
            if (right < 0 || sumFirst2[left] + sumLast2[right] != need) {
                ++left;
                continue;
            }
            for (newLeft = left; newLeft < U && sumFirst2[left] == sumFirst2[newLeft]; ++newLeft);
            for (newRight = right; newRight >= 0 && sumLast2[right] == sumLast2[newRight]; --newRight);

            answer += (newLeft - left) * (right - newRight);
            left = newLeft;
            right = newRight;
        }

        // now inplace_merge sumFirst2 with the new middle sums
        sumFirst2.resize(U + middle);
        int delta = middle;
        for (int i = U - 1, j = middle - 1; i >= 0 && j >= 0; --i) {
            if (A[middle] + A[j] > sumFirst2[i]) {
                sumFirst2[i + delta] = A[middle] + A[j];
                --delta; --j;
                ++i;
                continue;
            }
            sumFirst2[i + delta] = sumFirst2[i];
        }

        // now inplace_remove sumLast2 old sums
        for (int i = 0, j = middle + 2, k = 0; i < V; ++i) {
            if (j < N && sumLast2[i] == A[middle + 1] + A[j])
                ++j;
            else
                sumLast2[k++] = sumLast2[i];
        }
        sumLast2.resize(V - (N - middle - 2));
    }

    cout << answer << "\n";
}
