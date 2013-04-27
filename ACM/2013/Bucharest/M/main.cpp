#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <cassert>

using namespace std;

class SegmentTreeMax {
  public:
    SegmentTreeMax(const vector<int>& input) {
        size_ = 1;
        while (size_ < input.size())
            size_ *= 2;
        data_ = new int[size_ * 2];

        for (size_t i = 0; i < input.size(); ++i)
            data_[size_ + i] = input[i];
        for (size_t i = input.size(); i < size_; ++i)
            data_[size_ + i] = 0;
        
        for (size_t i = size_ - 1; i; --i)
            data_[i] = max(data_[i * 2], data_[i * 2 + 1]);
    }

    void update(int where, const int &value) {
        where += size_;
        data_[where] = value;
        for (where /= 2; where > 0; where /= 2)
            data_[where] = max(data_[where * 2], data_[where * 2 + 1]);
    }

    int query(int from, int to) {
        int answer = 0;
        from += size_;
        to += size_;
        while (from <= to) {
            answer = max(answer, data_[from]);
            from = (from + 1) / 2;

            answer = max(answer, data_[to]);
            to = (to - 1) / 2;
        }

        return answer;
    }

    void clear() {
        delete[] data_;
    }

  private:
    size_t size_;
    int* data_;
};

const int MAXN = 100 * 1000 + 5;
const int LIM = 280 * 1000 + 5;
const int INF = 1000;

int i, j, n, m, MinArb[LIM][26], MaxArb[LIM][26], minexp[27], maxexp[27];
int power[50][50], primeCount, cntCurrent[27];
int fact[50];

void arbQuery(int node, int left, int right, int x, int y) {

    if(x <= left && right <= y) {
        //cerr << left <<" " <<  right <<"   ";
        for(int i = 1; i <= primeCount; ++i) {
            minexp[i] = min(minexp[i], MinArb[node][i]);
            maxexp[i] = max(maxexp[i], MaxArb[node][i]);
            //cerr << maxexp[i] << " ";
        }
        //cerr << "\n";
        return;
    }
    
    int mid = (left + right) >> 1;

    if(x <= mid)
        arbQuery(2 * node, left, mid, x, y);
    if(y > mid)
        arbQuery(2 * node + 1, mid + 1, right, x , y);
}

int query(int left, int right, int mod, int g) {
    
    for(int i = 1; i <= primeCount; ++i)
        minexp[i] = INF, maxexp[i] = 0;

    arbQuery(1, 1, n, left, right);
    
    int ans = 1;

    for(int i = 1; i <= primeCount; ++i) {
        if(g) {
            if(minexp[i] == INF)
                continue;
            ans = ans * power[i][minexp[i]] % mod;
        }
        else {
            //cerr << i << " " << maxexp[i] << "\n";
            ans = ans * power[i][maxexp[i]] % mod;
        }
    }

    return ans % mod;
}


void update(int node, int left, int right, int index) {
    if(left == right) {
        for(int i = 1; i <= primeCount; ++i) 
            MinArb[node][i] = MaxArb[node][i] = cntCurrent[i];
        return;
    }

    int mid = (left + right) >> 1;

    if(index <= mid)
        update(2 * node, left, mid, index);
    else
        update(2 * node + 1, mid + 1, right, index);

    for(int i = 1; i <= primeCount; ++i) {
        MaxArb[node][i] = max(MaxArb[2 * node][i], MaxArb[2 * node + 1][i]);
        MinArb[node][i] = min(MinArb[2 * node][i], MinArb[2 * node + 1][i]);
    }
}

int main() {

    //freopen("M.in","r",stdin);
    
    for(i = 2; i <= 100; ++i) {
        bool ok = true;
        for(j = 2; j < i; ++j)
            if(i % j == 0) {
                ok = false;
                break;
            }

        if(ok)
            fact[++primeCount] = i;
    }
    
    for(i = 1; i <= primeCount; ++i) {
        power[i][0] = 1;
        for(j = 1; power[i][j - 1] * fact[i] <= 200; ++j)
            power[i][j] = power[i][j - 1] * fact[i];
    }

    for(;!feof(stdin);) {
        assert(scanf("%d %d", &n, &m) == 2);

        for(i = 1; i <= n; ++i) {
            int a;
            assert(scanf("%d", &a) == 1);
            baga(i, a);
            //cerr << i << "avem\n";
        }

        vector<SegmentTreeMax> maxArb(primeCount, SegmentTreeMax(n + 1));
        vector<SegmentTreeMin> minArb(primeCount, SegmentTreeMin(n + 1));
        for(i = 1; i <= m; ++i) {
            char c;
            assert(scanf("%c",&c) == 1);
            //cerr << c << " avem\n";
            int lf, rt;
            if(c == 'C') {
                int index, value;
                assert(scanf("%d %d", &index, &value) == 2);
                int aux = value;
                memset(cntCurrent, 0, sizeof(cntCurrent));

                for(int i = 1; i <= primeCount; ++i) {
                    while(aux % fact[i] == 0) {
                        cntCurrent[i]++;
                        aux /= fact[i];
                    }
        
                maxArb[i].update(index, cntCurrent[i]);
                minArb[i].update(index, cntCurrent[i]);
            }
            
            if(c == 'L') {
                int mod;
                assert(scanf("%d %d %d", &lf, &rt, &mod));
                cout << query(lf, rt, mod, 0) << "\n";
            }

            if(c == 'G') {
                int mod;
                assert(scanf("%d %d %d", &lf, &rt, &mod));
                cout << query(lf, rt, mod, 1) << "\n";
            }
        }
    }

    return 0;
}
