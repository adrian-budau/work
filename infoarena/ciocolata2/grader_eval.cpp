#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <set>
#include <vector>

using namespace std;

#define maxn 1010

FILE *in, *out, *ok;
int n, m, k, total;
int mark[maxn][maxn];
int block[maxn][maxn];
int val[maxn][maxn];
int unmarked, blocked, marked, blocked_and_marked;

void msg(int points, string msg)
{
    cerr<<msg;
    cout<<points;
    exit(0);
}

void corruptedInput()
{
    msg(0, "Fisier de intrare corupt!");
}

void corruptedOutput()
{
    msg(0, "Fisier de iesire corupt!");
}

int ab(int x)
{
    if(x<0)
        return -x;
    return x;
}

int cellOk(int x, int y)
{
    return (x>0 && y>0 && x<=n && y<=m);
}

int adjacent(int x1, int y1, int x2, int y2)
{
    return (ab(x1-x2)+ab(y1-y2)==1) && (val[x1][y1]==val[x2][y2]);
}

vector<pair<int, int> > readSet(FILE *f)
{
    vector<pair<int, int> > sol;
    int b;
    if(fscanf(f, "%d", &b)!=1)
        corruptedOutput();
    if(b==-1)
    {
        sol.push_back(make_pair(-1, -1));
        return sol;
    }
    int line = 0;
    while(b--)
    {
        ++line;
        int x1, y1, x2, y2;
        if(fscanf(f, "%d%d%d%d", &x1, &y1, &x2, &y2)!=4)
            corruptedOutput();
        if(!(cellOk(x1, y1) && cellOk(x2, y2)))
            msg(0, "WA: Bucata de ciocolata iese din caroiaj!");
        if((!adjacent(x1, y1, x2, y2)) && f == out) {
            cerr << "--> " << line << "\n";
            msg(0, "WA: Bucata de ciocolata acopera celule neadiacente!");
        }
        sol.push_back(make_pair(x1, y1));
        sol.push_back(make_pair(x2, y2));
    }
    return sol;
}

int readRemoveSet()
{
    vector<pair<int, int> > cells = readSet(out);

    if(cells.size() == 1)
    {
        if(cells[0].first == -1)
            return 1;
    }

    for(int i=0; i<cells.size(); ++i)
    {
        int x=cells[i].first, y=cells[i].second;
        if(mark[x][y]==0)
            msg(0, "WA: Eliminare a unei bucati inexistente!");
        mark[x][y]=0;
        if(block[x][y])
        {
            --blocked_and_marked;
            ++blocked;
        }
        else
        {
            --marked;
            ++unmarked;
        }
        val[x][y]=0;
    }
    return 0;
}

int readAddSet()
{
    vector<pair<int, int> > cells = readSet(out);

    if(cells.size() == 1)
    {
        if(cells[0].first == -1)
            msg(0, "WA: Terminarea executiei la moment gresit!");
    }

    for(int i=0; i<cells.size(); ++i)
    {
        if(i%2==0)
            ++total;
        int x=cells[i].first, y=cells[i].second;
        val[x][y]=total;
        if(block[x][y])
            msg(0, "WA: Adaugare unei bucati peste o celula marcata!");
        if(mark[x][y])
            msg(0, "WA: Adaugare unei bucati peste o alta bucata!");
        mark[x][y]=1;
        --unmarked;
        ++marked;
    }
    return 0;
}

void readBlockSet()
{
    int b;
    if(fscanf(in, "%d", &b)!=1)
        corruptedInput();
    while(b--)
    {
        int x, y;
        if(fscanf(in, "%d%d", &x, &y)!=2)
            corruptedInput();
        if(block[x][y])
            corruptedInput();
        block[x][y]=1;
        if(mark[x][y])
        {
            ++blocked_and_marked;
            --marked;
        }
        else
        {
            ++blocked;
            --unmarked;
        }
    }
}

void checkState()
{
    if(unmarked>0)
        msg(0, "WA: Celula nemarcata ramasa neacoperita!");
    if(blocked_and_marked>0)
        msg(0, "WA: Celula marcata ramasa acoperita!");
}

int main()
{
    in=fopen("ciocolata2.in", "r");
    out=fopen("ciocolata2.out", "r");
    ok=fopen("ciocolata2.ok", "r");

    if(in==NULL)
        msg(0, "Missing input file!");
    if(out==NULL)
        msg(0, "Missing output file!");
    if(ok==NULL)
        msg(0, "Missing ok file!");

    if(fscanf(in, "%d%d%d", &n, &m, &k)!=3)
        corruptedInput();

    unmarked = n*m;
    readAddSet();
    readSet(ok);
    checkState();

    for(int i=1; i<=k; ++i)
    {
        int should_end=0;
        readBlockSet();
        vector<pair<int, int> > from_ok = readSet(ok);
        if(from_ok.size()==1)
            if(from_ok[0].first==-1)
                should_end=1;
        if(!should_end)
            readSet(ok);

        int does_end = readRemoveSet();

        if(should_end != does_end)
            msg(0, "WA: Terminarea executiei la moment gresit!");

        if(should_end)
            break;

        readAddSet();
        checkState();
    }

    msg(10, "OK!");

    return 0;
}
