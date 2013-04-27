#include <cstdio>
#include <cmath>

#define ll long long
const int PRE = 7000 * 1001;

int n, d;
ll phi[PRE + 5];
ll sumphi[PRE + 5];

using namespace std;

void getPre() {
    
    int i, j;

    for(i = 1; i <= PRE; ++i)
        phi[i] = i;

    for(i = 2; i <= PRE; ++i)
        if(phi[i] == i)
            for(j = i; j <= PRE; j += i) {
                phi[j] /= i;
                phi[j] *= (i - 1);
            }
    
    for(i = 1; i <= PRE; ++i) {
        sumphi[i] = phi[i] + sumphi[i - 1];
        //fprintf(stderr,"%d\n",phi[i]);
    }
}

ll solve(int n) {
    
    if(n <= PRE)
        return sumphi[n];
    
    int ans = 0;
    return ans;
}

int main() {
    
    freopen("cntgcd.in","r",stdin);
    freopen("cntgcd.out","w",stdout);

    scanf("%d %d",&n,&d);

    getPre();
    printf("%lld\n", solve(n / d));
    
    return 0;
}
