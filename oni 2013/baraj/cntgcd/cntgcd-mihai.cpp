#include <cstdio>
#include <cmath>
#include <map>
#include <cmath>

using namespace std;

#define ll long long
const int PRE = 1000 * 1000;

int n, d;
ll phi[PRE + 5];
ll sumphi[PRE + 5];

map<int, ll> memo;

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
    
    sumphi[1] = 1;
    for(i = 2; i <= PRE; ++i) {
        sumphi[i] = 2 * phi[i] + sumphi[i - 1];
        //fprintf(stderr,"%d\n",phi[i]);
    }
}

ll solve(int n) {
    

    if(n <= PRE)
        return sumphi[n];
    
    if(n == 0)
        return 0;
    ll t = memo[n];
    
    if(t)
        return t;

    ll ans = 1LL * n * n;
    ll sqr = (sqrt((double)n));

    for(ll i = 2; i <= sqr; ++i)
        ans -= solve(n / i);
    
    ++sqr;
    for(ll i = n / sqr; i >= 1; --i) {
        ll left, right;
        right = n / i;
        left = n / (i + 1);
        ans -= 1LL * (right - left) * solve(i);
    }
    
    //fprintf(stderr,"%d %lld\n", n, ans);

    memo[n] = ans;
    return ans;
}

int main() {
    
    freopen("cntgcd.in","r",stdin);
    freopen("cntgcd.out","w",stdout);

    scanf("%d %d",&n,&d);

    getPre();
    memo[1] = 1;
    ll ans = solve(n / d);
    ans = (ans - 1) / 2;

    printf("%lld\n", ans + 1);
    
    return 0;
}
