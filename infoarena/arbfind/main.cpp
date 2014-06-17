#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class Tree {
  public:
    Tree(int size = 1):
            size_(size),
            sons_(size) {
    }

    void addSon(int father, int son) {
        sons_[father].push_back(son);
    }

    int size() const {
        return size_;
    }

    string euler() {
        position_ = vector<int>(size_);
        dfs(0);
        return euler_;
    }

    int position(int node) const {
        return position_[node];
    }

  private:
    void dfs(int node) {
        position_[node] = euler_.size();
        for (auto &next : sons_[node]) {
            euler_ += '+';
            dfs(next);
            euler_ += '-';
        }
    }

    int size_;
    vector< vector<int> > sons_;

    string euler_;
    vector<int> position_;
};

istream& operator>>(istream& input, Tree& object) {
    int size; input >> size;
    object = Tree(size);

    for (int i = 0; i < size; ++i) {
        int many; input >> many;
        while (many--) {
            int node; input >> node;
            object.addSon(i, node - 1);
        }
    }

    return input;
}

vector<int> zAlgorithm(string pattern, string text) {
    int N = pattern.size();
    string S = pattern + " " + text;

    int left = 0, right = 0;
    vector<int> match(S.size(), 0);
    for (int i = 1; i < int(S.size()); ++i) {
        if (right <= i) {
            left = i; right = i;
            while (right < int(S.size()) and S[right] == S[right - left])
                ++right;
            match[i] = right - left;
            continue;
        }

        match[i] = min(right - i, match[i - left]);
        if (i + match[i] == right) {
            left = i;
            while (right < int(S.size()) and S[right] == S[right - left])
                ++right;
            match[i] = right - left;
        }
    }

    return vector<int>(match.begin() + N + 1, match.end());
}

int main() {
    ifstream cin("arbfind.in");
    ofstream cout("arbfind.out");

    Tree A, B; cin >> A >> B;

    auto SA = A.euler(),
         SB = B.euler();

    auto match = zAlgorithm(SA, SB);

    vector<int> nodes(SA.size() + 1);
    nodes[0] = 1;
    for (int i = 1; i <= int(SA.size()); ++i)
        nodes[i] = nodes[i - 1] + (SA[i - 1] == '+');

    for (int i = 0; i < int(B.size()); ++i) {
        int length = match[B.position(i)];
        cout << nodes[length] << "\n";
    }
}
