#include <iostream>
#include<cstdio>
#include<cmath>
#include<map>
#define tip long long
using namespace std;
tip p[32005],fi[32005];
tip rad,SOL(tip);
map<tip,tip> M;
void ciur();
int main()
{
    freopen("cntgcd.in","r",stdin);
    freopen("cntgcd.out","w",stdout);
    tip n,d;
    scanf("%lld%lld",&n,&d);n/=d;rad=sqrt(n)+1;
    ciur();
    if(n<rad)
    {
        printf("%lld\n",fi[n]);
        return 0;
    }
    printf("%lld\n",(SOL(n)-1)/2+1);
    return 0;
}
void ciur()
{
    int i,j;
    for(i=2;i<=rad;i+=2)p[i]=2;
    for(i=3;i*i<=rad;i+=2)
    if(!p[i])
    {
        for(j=i*i;j<=rad;j+=2*i)
            p[j]=i;
    }
    fi[1]=1;
    for(i=2;i<=rad;i++)
    {
        if(!p[i]){fi[i]=i-1;continue;}
        fi[i]=p[i]-1;j=i/p[i];
        while(j%p[i]==0){fi[i]*=p[i];j/=p[i];}
        fi[i]*=fi[j];
    }
    for(i=1;i<=rad;i++)fi[i]+=fi[i-1];
}
tip SOL(tip x)
{
    tip ret,i,ST,DR,Rad=sqrt(x);
    if(x<rad)return 2*fi[x]-1;
    map<tip,tip>::iterator it=M.find(x);
    if(it!=M.end())return it->second;
    ret=x*x;
    for(i=2;x/i>Rad;i++)
        ret-=SOL(x/i);
    for(i=1;i<=Rad;i++)
    {
        DR=x/i;
        ST=x/(i+1);
        ret-=(DR-ST)*(2*fi[i]-1);
    }
    M[x]=ret;
    return ret;
}
