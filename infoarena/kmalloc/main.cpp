/**
 * (C) 2012 Bogdan-Cristian Tătăroiu
 */
#include <cstdio>
#include <cassert>
#include <algorithm>
#include <list>
 
using namespace std;
 
const int MAXN = 62;
const int MAXM = 500000;
const int MAXP = 100000;
 
#define int64 long long
 
int64 N;
int M, P;
list<int64> blocks[MAXN + 1];
 
inline void addFreeBlock(int64 start, int64 end) {
    if (start > end) {
        return;
    }
    int64 length = end - start + 1;
    int64 pos = start; int step = 0;
    for (; step <= MAXN && (1LL << step) <= length; step += 1) {
        if (length & (1LL << step)) {
            blocks[step].push_back(pos);
            pos += 1LL << step;
        }
    }
}
 
int main() {
    assert(scanf("%lld %d", &N, &P) == 2);
    int64 lastEnd = -1;
    for (int i = 0; i < P; i++) {
        int64 curStart, curEnd;
        assert(scanf("%lld %lld", &curStart, &curEnd) == 2);
        assert(curStart <= curEnd);
        assert(lastEnd < curStart);
 
        addFreeBlock(lastEnd + 1, curStart - 1);
        lastEnd = curEnd;
    }
    addFreeBlock(lastEnd + 1, N - 1);
 
    for (M = 0; ; M++) {
        int pow;
        assert(scanf("%d", &pow) == 1);
        if (pow == -1) {
            return 0;
        }
        assert(0 <= pow && pow <= MAXN);
 
        if (blocks[pow].empty()) {
            int it = pow;
            for (; it <= MAXN && blocks[it].empty(); it++);
            assert(it <= MAXN);
 
            for (; it > pow; it--) {
                int64 pos = blocks[it].front();
                blocks[it].pop_front();
 
                blocks[it - 1].push_back(pos);
                blocks[it - 1].push_back(pos + (1LL << (it - 1)));
            }
        }
 
        assert(!blocks[pow].empty());
        printf("%lld\n", blocks[pow].front());
        blocks[pow].pop_front();
        fflush(stdout);
    }
    assert(M <= MAXM);
    return 0;
}
