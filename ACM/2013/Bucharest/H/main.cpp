#include <cstdio>
#include <map>
#include <vector>
#include <cassert>
#include <algorithm>

using namespace std;

inline int get(map<int, int> &M, int x) {
    if (M.find(x) != M.end())
        return M[x];
    int v = M.size();
    return (M[x] = v);
}

int main() {
    int x, y;
    int test = 0;
    do {
        ++test;
        assert(scanf("%d %d", &x, &y) == 2);
        if (x < -1 && y < -1)
            break;

        vector<int> A;
        map<int, int> M;
        bool bad = false;
        int edges = 0;
        do {
            if (x == 0 && y == 0)
                break;
            ++edges;
            x = get(M, x); y = get(M, y);
            if (x >= int(A.size()) || y >= int(A.size()))
                A.resize(max(x, y) + 1, -1);
            if (A[y] != -1) {
                bad = true;
            }
            A[y] = x;
            assert(scanf("%d %d", &x, &y) == 2);
        } while (1);

       if (A.size() == 0) {
            printf("Case %d is a tree.\n", test);
            continue;
        }

        if (bad || edges != int(A.size()) - 1)  {
            printf("Case %d is not a tree.\n", test);
            continue;
        }
 
        if (count(A.begin(), A.end(), -1) != 1) {
            printf("Case %d is not a tree.\n", test);
            continue;
        }

        int root = min_element(A.begin(), A.end()) - A.begin();
        
        vector<int> type(A.size(), 0);
        type[root] = 2;
        for (int i = 0; i < int(A.size()); ++i)
            if (type[i] == 0) {
                type[i] = 0;
                int j;
                for (j = i; type[j] == 0; j = A[j])
                    type[j] = 1;

                if (type[j] == 1)
                    break;
                for (j = i; type[j] == 1; j = A[j])
                    type[j] = 2;
            }
        
        if (*min_element(type.begin(), type.end()) == 2) {
            printf("Case %d is a tree.\n", test);
        } else {
            printf("Case %d is not a tree.\n", test);
        }
    } while (1);
}
