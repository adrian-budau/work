#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <queue>

using namespace std;

class Queue {
  public:
    Queue():
            start_(0) {
    }

    int front() const {
        return data_[start_];
    }

    void push(int x) {
        data_.push_back(x);
    }

    void pop() {
        ++start_;
    }

  private:
    int start_;
    vector<int> data_;
};

vector<int> rotate_min(vector<int> V) {
    vector<int> S = V;
    int N = V.size();
    S.insert(S.end(), V.begin(), V.end());

    unordered_map<int, int> H;
    auto add = [&](int x, int i) {
        if (H.find(x) != H.end())
            return H[x];
        return (H[x] = i);
    };

    for (int i = 0; i < int(S.size()); ++i) {
        S[i] = add(S[i], i);
    }

    vector<int> pi(S.size(), 0);
    int k = 0;
    int best = 0;

    vector<Queue> Q(*max_element(S.begin(), S.end()) + 1), Q2(*max_element(S.begin(), S.end()) + 1);
    Q2[S[0]].push(0);
    for (int i = 1; i < int(S.size()) - 1; ++i) {
        Q[S[i]].push(i);
        Q2[S[i]].push(i);
        // the match is from i - k to i - 1
        while (k > 0 and Q[S[i]].front() - (i - k) != Q2[S[k + best]].front() - best) {
            int next = pi[k - 1];
            if (Q[S[i]].front() - (i - k) < Q2[S[k + best]].front() - best) {
                for (; best < i - k; ++best)
                    Q2[S[best]].pop();
            }

            for (; k > next; --k)
                Q[S[i - k]].pop();
        }
 
        if (Q[S[i]].front() - (i - k) == Q2[S[k + best]].front() - best)
            ++k;

        pi[i - best] = k;
    }

    vector<int> H2(S.size(), -1);
    int many = 0;
    auto add2 = [&](int x) {
        if (H2[x] != -1)
            return H2[x];
        return (H2[x] = many++);
    };

    vector<int> answer(S.begin() + best, S.begin() + best + N);
    for (int i = 0; i < int(answer.size()); ++i) {
        answer[i] = add2(answer[i]);
    }
    return answer;
}

int main() {
    ios::sync_with_stdio(false);
    int T; cin >> T;

    while (T--) {
        int N, K; cin >> N >> K;

        vector< vector<int> > V(N, vector<int>(K, 0));
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < K; ++j)
                cin >> V[i][j];
            V[i] = rotate_min(V[i]);
        }
        sort(V.begin(), V.end());
        V.erase(unique(V.begin(), V.end()), V.end());
        cout << V.size() << "\n";
    }
}
