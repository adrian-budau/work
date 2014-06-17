//brute O(n ^ 2) fara optimizare
 
#include<stdio.h>
 
#define ll long long
#define NMAX 100005
 
int n,m;
int z[NMAX];
ll s;
 
int main ()
{
    int i,j,tip,poz,val,b;
 
    freopen("kami.in","r",stdin);
    freopen("kami.out","w",stdout);
 
    scanf("%d",&n);
    for(i = 1; i <= n; i++)
        scanf("%d",&z[i]);
 
    scanf("%d",&m);
    for(i = 1; i <= m; i++)
    {
        scanf("%d",&tip);
        if(!tip)
        {
            scanf("%d%d",&poz,&val);
            z[poz] = val;
            continue;
        }
        scanf("%d",&b);
        s = z[b];
        for(j = b - 1; j >= 1; j--)
            if(z[j] >= s)
                break;
            else
                s += z[j];
        printf("%d\n",j);
    }
 
    return 0;
}
