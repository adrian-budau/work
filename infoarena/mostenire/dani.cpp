#include<stdio.h>
#include<algorithm>
using namespace std;

#define ll long long
#define NMAX 1000005
#define pi pair< ll, ll >
#define x first
#define y second
#define mp make_pair

int indexMin, indexMax;
int v[NMAX],n;
ll K,numNephew,sp[NMAX], Give[NMAX];
pi sol;

inline int NeedBinarySearch(ll S, ll D)
{
    int st = 1, dr = n, mij, sol;
    while(st <= dr)
    {
        mij = (st + dr) / 2;
        if(S * v[mij] >= S * v[n] - D)
        {
            dr = mij - 1;
            sol = mij;
        }
        else
            st = mij + 1;
    }
    return sol;
}

inline int Analizeaza(ll S, ll D)
{
    if(S * v[1] + D * v[1] < S * v[n] - D)
        return 1;
    ll T = K - S * numNephew;

    int indexNeed = NeedBinarySearch(S,D);
    ll need = (S * v[n] - D) * (indexNeed - 1) - S * sp[indexNeed - 1];
    if(need > T)
        return 1;

    ll tiganeala = D * v[1] * indexMin + ((S + D) * v[1] + D) * (n - indexMin) - S * (sp[n] - sp[indexMin]);
    if(tiganeala < T)
        return -1;
    return 0;
}

inline ll NephewBinarySearch(ll D)
{
    ll st = 0, dr = K / numNephew, mij;
    int ans;

    while(st <= dr)
    {
        mij = (st + dr) / 2;
        ans = Analizeaza(mij, D);
        //printf("pai %lld %d\n", mij,ans);
        if(!ans)
            return mij;
        if(ans == -1)
            st = mij + 1;
        else
            dr = mij - 1;
    }
    return -1;
}

inline pi DiffBinarySearch()
{
    ll st = 0, dr = K, mij;
    ll ans;
    pi sol;

    while(st <= dr)
    {
        mij = (st + dr) / 2;
        //printf("%lld\n", mij);
        ans = NephewBinarySearch(mij);
        if(ans == -1)
            st = mij + 1;
        else
        {
            sol = mp(mij, ans);
            dr  = mij - 1;
        }
    }
    return sol;
}

inline void GetIndex()
{
    indexMin = 1; indexMax = n;
    while(indexMin < n && v[indexMin + 1] == v[indexMin])
        indexMin++;
    while(indexMax > 1 && v[indexMax - 1] == v[indexMax])
        indexMax--;
}

inline int Works(ll valMin, ll D, ll S)
{
    int i;
    ll T = K, maxGive;

    for(int i = 1; i <= n; i++)
    {
        if(valMin < S * v[i])
            Give[i] = S * v[i];
        else
            Give[i] = valMin;
        T -= Give[i];
    }
    if(T < 0)
        return 1;
    for(i = 1; i <= n; i++)
    {
        maxGive = min((S + D) * v[i], valMin + D) - Give[i];
        if(T <= maxGive)
        {
            Give[i] += T;
            return 0;
        }
        else
        {
            T -= maxGive;
            Give[i] += maxGive;
        }
    }
    return -1;
}

inline void ReconstBinarySearch(ll D, ll S)
{
    ll st = max(S * v[1], S * v[n] - D), dr = (S + D) * v[1], mij;
    int ans;
    //printf("%lld %lld\n",D,S);
    while(st <= dr)
    {
        mij = (st + dr) / 2;
      //  printf("pai %lld\n",mij);
        ans = Works(mij, D, S);
        if(!ans)
            return ;
        if(ans == -1)
            st = mij + 1;
        else
            dr = mij - 1;
    }
}

int main ()
{
    int i;

    freopen("mostenire.in","r",stdin);
    freopen("mostenire.out","w",stdout);

    scanf("%d%lld",&n,&K);
    for(i = 1; i <= n; i++)
    {
        scanf("%d",&v[i]);
        numNephew += v[i];
        sp[i] = sp[i - 1] + v[i];
    }

    GetIndex();

    sol = DiffBinarySearch();
    //ll ans = NephewBinarySearch(2);
    //ll ans = Analizeaza(0,2);
    //printf("%lld\n",ans);
    ReconstBinarySearch(sol.x, sol.y);

    printf("%lld\n",sol.x);

    for(i = 1; i <= n; i++)
        printf("%lld ",Give[i]);
    printf("\n");
    return 0;
}

