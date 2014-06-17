#include<stdio.h>
#include<algorithm>
#include<math.h>
#include<cmath>
#include<string>
#include<iostream>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <limits>
#include <queue>
#include <stdexcept>
 
#define PI acos(-1)
using namespace std;
 
const double kInfinite = 1e40;
const double eps = 1e-6;

inline int cmp(double a, double b) {
	if (a + eps < b)
		return -1;
	if (b + eps < a)
		return 1;
	return 0;
}

class BipartiteGraph {
  public:
    BipartiteGraph(int size_l, int size_r):
            size_l_(size_l),
            size_r_(size_r),
            edges_(size_l),
            leftMatch(size_l, -1),
            rightMatch(size_r, -1),
            costLeft(size_l, 0),
            costRight(size_r, 0) {
    }
 
    void addEdge(int from, int to, double distance) {
        edges_[from].push_back(Edge(to, distance));
    }
 
    int nodeMatch(int node) {
        return leftMatch[node];
    }
 
    pair<int, double> minimumWeightMatching() {
        int flowSize = 0;
 
        while (true) {
            vector<bool> usedLeft(size_l_, false), usedRight(size_r_, false);
            vector<double> tillEdge(size_r_, kInfinite);
            queue<int> Q;
            for (int i = 0; i < size_l_; ++i)
                if (leftMatch[i] == -1) {
                    usedLeft[i] = true;
                    Q.push(i);
                }
 
            auto updateRight = [&](int node) {
                usedRight[node] = true;
				
 
                // go left if possible
                if (rightMatch[node] != -1) {
                    tillEdge[node] = kInfinite;
                    usedLeft[rightMatch[node]] = true;
                    Q.push(rightMatch[node]);
                }
            };
 
            do {
                while (!Q.empty()) {
                    int node = Q.front();
                    Q.pop();
 
                    // pass the graph
                    for (auto &next : edges_[node]) {
                        if (not usedRight[next.to])
                             if (cmp(costLeft[node] + costRight[next.to], next.distance) == 0
                                 && next.to != leftMatch[node])
                                    updateRight(next.to);
                        if (not usedRight[next.to] or rightMatch[next.to] == -1)
                            tillEdge[next.to] = min(tillEdge[next.to], next.distance - costLeft[node] - costRight[next.to]);
                    }
                }
 
                int nextChange = min_element(tillEdge.begin(), tillEdge.end()) - tillEdge.begin();
                double change = tillEdge[nextChange];
                if (change >= kInfinite / 10)
                    break;
                // we're still stuck here, let's extend
                for (int i = 0; i < size_l_; ++i)
                    if (usedLeft[i])
                        costLeft[i] += change;
 
                for (int i = 0; i < size_r_; ++i)
                    if (usedRight[i])
                        costRight[i] -= change;
                    else if (tillEdge[i] < kInfinite / 10)
                        tillEdge[i] -= change;
 
                if (rightMatch[nextChange] == -1)
                    break;
                updateRight(nextChange);
            } while (true);
 
            // yey we got a new edge, maybe?
            bool didWe = false;
            used = vector<bool>(size_l_, false);
            for (int i = 0; i < size_l_; ++i)
                if (leftMatch[i] == -1)
                    if (match(i)) {
                        ++flowSize;
                        didWe = true;
                    }
            if (not didWe)
                break;
        }
 
        // and lucky for us the sum of y's is the answer
        double answer = 0;
        for (int i = 0; i < size_l_; ++i)
            if (leftMatch[i] != -1)
                answer += costLeft[i];
        for (int i = 0; i < size_r_; ++i)
            if (rightMatch[i] != -1)
                answer += costRight[i];
        return make_pair(flowSize, answer);
    }
 
  private:
    class Edge {
      public:
        Edge(int _to, double _distance):
                to(_to), distance(_distance) {}
        int to;
        double distance;
    };
 
    // hopcroft-karp style
    bool match(int node) {
        if (used[node])
            return false;
        used[node] = true;
        for (auto &next : edges_[node])
            if (cmp(costLeft[node] + costRight[next.to], next.distance) == 0) {
                if (rightMatch[next.to] == -1) {
                    rightMatch[next.to] = node;
                    leftMatch[node] = next.to;
                    return true;
                }
 
                if (match(rightMatch[next.to])) {
                    rightMatch[next.to] = node;
                    leftMatch[node] = next.to;
                    return true;
                }
            }
        return false;
    }
 
    int size_l_, size_r_;
    vector< vector<Edge> > edges_;
 
    vector<int> leftMatch, rightMatch;
    vector<double> costLeft, costRight;
 
    vector<bool> used;
};
 
struct punct { double a, b; int index;};

bool cmp2( const punct &p1, const punct &p2) {
	return p1.b < p2.b;
}

punct puncte[3001];

double rad( double degree) {
  return degree * (PI/180);
}

double calc_dist( int poz1, int poz2) {
	
	double lat1 = puncte[poz1].a, lat2 = puncte[poz2].a, lon1 = puncte[poz1].b, lon2 = puncte[poz2].b;
	double R = 6371; // km
	double dLat = rad((lat2-lat1));
	double dLon = rad((lon2-lon1));
	lat1 = rad(lat1);
	lat2 = rad(lat2);
	
	double a = sin(dLat/2) * sin(dLat/2) +
			sin(dLon/2) * sin(dLon/2) * cos(lat1) * cos(lat2); 
	double c = 2 * atan2(sqrt(a), sqrt(1-a)); 
	double d = R * c;

    return d;
}

int main() {

	
	
	//for( int i = 0; i <= 10; ++i)	{
	
	string text, text2;
	/*text += string(i); text += ".in";
	text2 += string(i); text2 += ".ouut";
	freopen( text, "r", stdin);
	freopen( text2, "w", stdout);
	*/
	freopen("input", "r", stdin);
	freopen("E0.out", "w", stdout);
	
	int n;
	cin>>n;
	
	for( int i = 0; i < n; ++i) {
	
		cin>>puncte[i].a>>puncte[i].b;
		puncte[i].index = i;
	}
	sort( puncte, puncte + n, cmp2);
	
	puncte[n] = puncte[0];
	double sum = 0;
	
	for( int i = 1; i <= n; ++i) {
			
			double d = calc_dist(i, i-1);
			sum += d;
			
	}
	
	double optsum = sum; 

    vector<int> optsol;    
	for( int i = 0; i < n; ++i)
		optsol.push_back(puncte[i].index);
	//cout<<puncte[n].index<<endl;

    cerr << optsum << "\n";
	for( int mijloc = 1 ; mijloc < n; ++mijloc) 
        for (int capat = 1; capat < n; ++capat) 
            for (int capat2 = 1; capat2 < n; ++capat2) {
        if (capat == mijloc)
            continue;
        if (capat2 == mijloc || capat2 == capat)
            continue;
        if (puncte[capat].b - puncte[0].b <= 180)
            continue;
        if (puncte[capat2].b - puncte[mijloc].b <= 180)
            continue;
		BipartiteGraph G(n,n);
		for( int i = 0; i < n; ++i) {
            if (i == capat || i == capat2)
                continue;
			for( int j = i + 1; j < n; ++j) {
				if( puncte[j].b - puncte[i].b <= 180) {
					G.addEdge( puncte[i].index, puncte[j].index, calc_dist(i,j));
				}
			}
		}

        int flow;
		double now;
        tie(flow, now) = G.minimumWeightMatching();
        now += calc_dist(capat, 0) + calc_dist(capat2, mijloc);
        cerr << flow << " " << now << "\n";
        if (flow == n - 2 && cmp(now, optsum) == -1) {
            optsum = now;
            optsol.clear();
            int node = puncte[0].index;
            do {
                optsol.push_back(node);
                node = G.nodeMatch(node);
            } while (node != -1);
        }
	}

	cout<<optsum << "\n";
	
    for (auto &x : optsol)
        cout << x << " " ;
    cout << "\n";
	return 0;
}
