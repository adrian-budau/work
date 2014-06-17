#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
 
using namespace std;
 
inline int anc(vector<int> &A, int x) {
    int y;
    for (y = x; y != A[y]; y = A[y]);
 
    int aux;
    for (;x != y; x = aux) {
        aux = A[x];
        A[x] = y;
    }
 
    return y;
}
 
class functor {
  public:
    functor(const vector< pair<int, int> > &_buffer): buffer(_buffer) {}
     
    bool operator()(const int &x, const int &y) const {
        return buffer[x] < buffer[y];
    }
 
  private:
    const vector< pair<int, int> > &buffer;
};
 
int main() {
    ifstream cin("banana.in");
    ofstream cout("banana.out");
 
    int N, K; cin >> N >> K;
 
    vector< pair<int, int> > S(N);
    for (int i = 0; i < N; ++i)
        cin >> S[i].first >> S[i].second;
 
    vector<int> group(N);
    for (int i = 0; i < N; ++i)
        group[i] = i;
 
    vector<int> size(N, 1);
 
    vector<int> P = group;
 
    // unite horizontal
    sort(P.begin(), P.end(), functor(S));
    for (int i = 1; i < N; ++i)
        if (S[P[i - 1]].first == S[P[i]].first && S[P[i - 1]].second == S[P[i]].second - 1) {
            int x = anc(group, P[i - 1]);
            int y = anc(group, P[i]);
 
            if (x != y) {
                group[x] = y;
                size[y] += size[x];
                size[x] = 0;
                //cerr << "Horizontal -> " << P[i - 1] << " " << P[i] << "\n";
            }
        }
 
    // unite vertical
    for (int i = 0; i < N; ++i)
        swap(S[i].first, S[i].second);
    sort(P.begin(), P.end(), functor(S));
 
    for (int i = 1; i < N; ++i)
        if (S[P[i - 1]].first == S[P[i]].first && S[P[i - 1]].second == S[P[i]].second - 1) {
            int x = anc(group, P[i - 1]);
            int y = anc(group, P[i]);
 
            if (x != y) {
                group[x] = y;
                size[y] += size[x];
                size[x] = 0;
                //cerr << "Vertical -> " << P[i - 1] << " " << P[i] << "\n";
            }
        }
 
 
    sort(size.begin(), size.end()); 
    reverse(size.begin(), size.end());
 
    int answer = 0;
    for (int i = 0; i < K; ++i)
        answer += size[i];
 
    cout << answer;
}
