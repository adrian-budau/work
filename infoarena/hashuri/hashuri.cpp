#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

const int kBucketCount = (1 << 21);
const int prime = 41;

int buckets[kBucketCount];

inline int next(const int &number) {
    return ((5 * number) + 1) % kBucketCount;
}

inline int search(const int &number) {
    int hash = ((number % kBucketCount) * prime) % kBucketCount;
    while (buckets[hash]) {
        if (buckets[hash] == number) 
            return hash;

        hash = next(hash);
    }

    return -1;
}

inline void insert(const int &number) {
    int hash = ((number % kBucketCount) * prime) % kBucketCount;
    while (buckets[hash] > 0) {
        hash = next(hash);
    }

    buckets[hash] = number;
}

int main() {
    ifstream cin("hashuri.in");
    ofstream cout("hashuri.out");

    int N; cin >> N;
    for (int i = 0; i < N; ++i) {
        int operation; cin >> operation;
        int number; cin >> number;

        switch (operation) {
            case 1:
                if (search(number) < 0)
                    insert(number);
                break;
            case 2:
                int x;
                if ((x = search(number)) >= 0)
                    buckets[x] = -1;
                break;
            case 3:
                cout << int(search(number) >= 0) << "\n";
        }
    }
}
