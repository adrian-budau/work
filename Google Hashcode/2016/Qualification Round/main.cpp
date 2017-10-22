#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>
#include <map>
#include <cmath>

using namespace std;

static const int kMaxLookout = 10;
static const double powTime = 1.4;
static const double powRatio = 1;

int rows, cols, D, T, capacity;
int points = 0;

int products;
vector<int> weights;

struct Warehouse {
    int row, col;
    vector<int> capacity;
};

int wh;
vector<Warehouse> W;

int customers;
struct Customer {
    int row, col;
    int index;
    multiset<int> need;
};

vector<Customer> C;

struct Drone {
    int row, col;
    int time;
};

vector<Drone> where;

template<class T1, class T2>
int distance(T1 a, T2 b) {
    return ceil(sqrt((a.row - b.row) * (a.row - b.row) + (a.col - b.col) * (a.col - b.col)));
}

struct Pick {
    double cost; int time;
    int warehouse, customer;
    map<int, int> take;

    bool operator<(const Pick& that) const {
        if (cost != that.cost)
            return cost < that.cost;
        if (warehouse != that.warehouse)
            return warehouse < that.warehouse;
        return customer < that.customer;
    }
};
Pick efficienyTransport(int warehouse, int customer) {
    int time = distance(W[warehouse], C[customer]);
    auto v = vector<int>(C[customer].need.begin(), C[customer].need.end());
    random_shuffle(v.begin(), v.end());
    int left = capacity;
    map<int, int> take;
    int transporting = 0, total = 0;
    for (auto &item : v) {
        total += weights[item];
        if (weights[item] <= left && W[warehouse].capacity[item] - take[item] > 0) {
            take[item]++;
            left -= weights[item];
            transporting += weights[item];
        }
    }
    vector<int> types;
    for (auto &p : take)
        types.push_back(p.first);
    for (auto &v : types)
        if (take[v] == 0)
            take.erase(v);

    time += 2 * take.size();
    double cost = 1e50;
    if (transporting != 0)
        cost = pow(time, powTime) * pow(double(total) / double(transporting), powRatio);
    return Pick{cost, time, warehouse, customer, take};
}

template <typename T>
constexpr T sqrt_helper(T x, T lo, T hi)
{
  if (lo == hi)
    return lo;

  const T mid = (lo + hi + 1) / 2;
  if (x / mid < mid)
    return sqrt_helper<T>(x, lo, mid - 1);
  else
    return sqrt_helper(x, mid, hi);
}

template <typename T>
constexpr T ct_sqrt(T x)
{
  return sqrt_helper<T>(x, 0, x / 2 + 1);
}

template <typename T>
T isqrt(T x)
{
  T r = (T) std::sqrt((double) x);
  T sqrt_max = ct_sqrt(std::numeric_limits<T>::max());

  while (r < sqrt_max && r * r < x)
    r++;
  while (r > sqrt_max || r * r > x)
    r--;

  return r;
}

vector<set<Pick> > best, bestDrone;

pair<int, Pick> bestPick() {
    Pick v = *bestDrone[0].begin();
    int where = 0;
    for (int i = 1; i < D; ++i)
        if (bestDrone[i].begin()->cost < v.cost) {
            v = *bestDrone[i].begin();
            where = i;
        }
    return {where, v};
}

struct Command {
    int drone;
    char op;
    int to;
    int type;
    int number;
};

vector<Command> answer;

void remove(multiset<int> &A, vector<int> B) {
    for (auto &x : B)
        A.erase(A.find(x));
}

int main(int argc, char **argv) {
    //ifstream cin("2.in");
    cin >> rows >> cols >> D >> T >> capacity;

    cin >> products;
    weights.resize(products);
    for (int i = 0; i < products; ++i)
        cin >> weights[i];

    cin >> wh;
    W.resize(wh);
    for (int i = 0; i < wh; ++i) {
        cin >> W[i].row >> W[i].col;
        W[i].capacity.resize(products);
        for (int j = 0; j < products; ++j) {
            int x; cin >> x;
            W[i].capacity[j] = x;
        }
    }

    where.resize(D);
    for (int i = 0; i < D; ++i)
        where[i] = {W[0].row, W[0].col, 0};
    cin >> customers;
    C.resize(customers);
    for (int i = 0; i < customers; ++i) {
        cin >> C[i].row >> C[i].col;
        C[i].index = i;
        int L; cin >> L;
        for (int j = 0; j < L; ++j) {
            int v; cin >> v;
            C[i].need.insert(v);
        }
    }

    best.resize(wh);
    bestDrone.resize(D);
    ofstream aux(argv[1]);
    while (true) {
        for (int i = 0; i < wh; ++i) {
            best[i].clear();
            for (int j = 0; j < customers; ++j) {
                best[i].emplace(efficienyTransport(i, j));
                while (best[i].size() > kMaxLookout)
                    best[i].erase(*best[i].rbegin());
            }
        }

        for (int k = 0; k < D; ++k)
            bestDrone[k].clear();

        for (int i = 0; i < wh; ++i)
            for (auto &deal : best[i])
                for (int k = 0; k < D; ++k) {
                    Pick changed = deal;
                    changed.cost /= pow(changed.time, powTime);
                    changed.time += distance(W[i], where[k]) + where[k].time;
                    changed.cost *= pow(changed.time, powTime);
                    bestDrone[k].insert(changed);
                }

        auto b = bestPick();
        if (b.second.time > T)
            break;
        where[b.first] = {C[b.second.customer].row, C[b.second.customer].col, b.second.time};
        for (auto &p : b.second.take)
            answer.push_back(Command{b.first, 'L', b.second.warehouse, p.first, p.second});
        for (auto &p : b.second.take)
            answer.push_back(Command{b.first, 'D', C[b.second.customer].index, p.first, p.second});
        for (auto &p : b.second.take)
            W[b.second.warehouse].capacity[p.first] -= p.second;
        vector<int> delivered;
        for (auto &p : b.second.take)
            for (int i = 0; i < p.second; ++i)
                delivered.push_back(p.first);
        sort(delivered.begin(), delivered.end());
        remove(C[b.second.customer].need, delivered);
        if (C[b.second.customer].need.size() == 0) {
            swap(C[b.second.customer], C.back());
            C.pop_back();
            --customers;
            points += (double(T - b.second.time) / T * 100 + 0.5);
        }
        if (customers == 0)
            break;
        cerr << customers << " customers left at time " << b.second.time << " and " << points << " points " << "\n";
        auto c = answer.back();
        aux << c.drone << " " << c.op << " " << c.to << " " << c.type << " " << c.number << "\n";
        aux.flush();
    }

    cout << answer.size() << "\n";
    for (auto &c : answer)
        cout << c.drone << " " << c.op << " " << c.to << " " << c.type << " " << c.number << "\n";
}
