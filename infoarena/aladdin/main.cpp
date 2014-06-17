#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <exception>
#include <stack>

using namespace std;

class SAT {
  public:
    SAT(int size = 1):
        size_(size),
        edges_(size * 2) {
    }

    void set(int node, bool value) {
        clause(node, value, node, value);
    }

    void clause(int first_node, bool first_value, int second_node, bool second_value) {
        edges_[node(first_node, not first_value)].push_back(node(second_node, second_value));
        edges_[node(second_node, not second_value)].push_back(node(first_node, first_value));
    }

    bool solve() {
        value_ = vector<int>(size_, -1);

        lowlink_ = index_ = vector<int>(size_ * 2, 0);
        current_index_ = 0;

        component_ = vector<int>(size_ * 2, 0);
        for (int i = 0; i < size_ * 2; ++i)
            if (not lowlink_[i])
                dfsTarjan(i);

        for (int i = 0; i < size_; ++i)
            if (component_[i] == component_[i + size_])
                return false;

        reverse(components_.begin(), components_.end());
        for (auto &component : components_) {
            for (auto &node : component) {
                int real_node = node >= size_ ? node - size_ : node;
                if (value_[real_node] != -1)
                    break;
                if (node >= size_)
                    value_[real_node] = 1;
                else
                    value_[real_node] = 0;
            }
        }
        return true;
    }

    bool answer(int index) const {
        return value_[index];
    }

  private:
    int node(int key, bool value) const {
        if (value == true)
            return key;
        return key + size_;
    }

    void dfsTarjan(int node) {
        index_[node] = lowlink_[node] = ++current_index_;
        nodes_.push(node);

        for (auto &next : edges_[node])
            if (!lowlink_[next]) {
                dfsTarjan(next);
                lowlink_[node] = min(lowlink_[node], lowlink_[next]);
            } else if (index_[next])
                lowlink_[node] = min(lowlink_[node], lowlink_[next]);

        if (index_[node] == lowlink_[node]) {
            int now;
            vector<int> current_component;
            do {
                now = nodes_.top();
                nodes_.pop();
                index_[now] = 0;
                component_[now] = components_.size();
                current_component.push_back(now);
            } while (now != node);
            components_.push_back(current_component);
        }
    }

    int size_;
    vector< vector<int> > edges_;

    vector<int> lowlink_, index_;
    int current_index_;
    stack<int> nodes_;

    vector<int> component_;
    vector< vector<int> > components_;

    vector<int> value_;
};

class NoMagicException : public std::exception {
  public:
    NoMagicException(string&& message = "Magic failed"):
            message_(std::forward<string>(message)) {
    }

    const char * what() const throw() {
        return message_.c_str();
    }

  private:
    string message_;
};

class Aladdin {
  public:
    Aladdin(int rows = 1, int cols = 1):
            rows_(rows),
            cols_(cols),
            square_(rows, vector<int>(cols, 0)) {
        if (rows > 1000)
            throw NoMagicException("Too many rows, Aladdin can only handle at most 1000 rows");
        if (cols > 1000)
            throw NoMagicException("Too many columns, Aladdin can only handle at most 1000 columns");
    }

    void set(int x, int y, int value) {
        if (x < 0 or x >= rows_ or y < 0 or y >= cols_)
            throw NoMagicException("Magic set failed, (" + to_string(x) + ", " + to_string(y) + ") is too far for Aladdin");
        if (value < 0 || value > 4)
            throw NoMagicException("Magic set failed, Aladdin can only set values between 0 and 4");
        square_[x][y] = value;
    }

    const vector<bool>& operator[](int index) const {
        if (not magicWorked)
            throw NoMagicException("Magic failed, Aladdin is sorry");
        return answer_[index];
    }

    void magic() {
        for (int i = 0; i < 2; ++i) {
            square_[0][0] = i;
            if (tryMagic()) {
                magicWorked = true;
                break;
            }
        }
        if (not magicWorked)
            throw NoMagicException("Magic failed, Aladdin is very sorry :-(");
    }

  private:
    int row(int x) const {
        return x - 1;
    }

    int col(int x) const {
        return x + rows_ - 2;
    }

    bool tryMagic() {
        auto edge = square_; // we keep information about row i and column j
        SAT S(rows_ + cols_ - 2);

        for (int i = 1; i < rows_; ++i)
            for (int j = 1; j < cols_; ++j) {
                edge[i][j] -= edge[i - 1][j] + edge[i][j - 1] + edge[i - 1][j - 1];

                pair<int, bool> nodeA(i, true), nodeB(j, true);
                if (j % 2 == 0)
                    nodeA.second = not nodeA.second;
                if (i % 2 == 0)
                    nodeB.second = not nodeB.second;

                int now = edge[i][j];
                if (not nodeA.second) {
                    nodeA.second = not nodeA.second;
                    nodeB.second = not nodeB.second;
                    now = - (now - 1);
                }

                if (nodeB.second) { // now - 1 <= nodeA + nodeB <= now
                    switch (now) {
                        case 3:
                            S.set(row(i), true);
                            S.set(col(j), true);
                            break;
                        case 2:
                            S.clause(row(i), true, col(j), true);
                            break;
                        case 1:
                            S.clause(row(i), false, col(j), false);
                            break;
                        case 0:
                            S.set(row(i), false);
                            S.set(col(j), false);
                            break;
                        default:
                            return false;
                    }
                } else {
                    // now - 1 <= nodeA - nodeB <= now
                    switch (now) {
                        case 2:
                            S.set(row(i), true);
                            S.set(col(j), false);
                            break;
                        case 1:
                            S.clause(row(i), true, col(j), false);
                            break;
                        case 0:
                            S.clause(row(i), false, col(j), true);
                            break;
                        case -1:
                            S.set(row(i), false);
                            S.set(col(j), true);
                            break;
                        default:
                            return false;
                    }
                }
            }
        if (not S.solve())
            return false;

        answer_ = vector< vector<bool> > (rows_, vector<bool>(cols_, 0));
        answer_[0][0] = square_[0][0];
        for (int i = 1; i < rows_; ++i)
            answer_[i][0] = S.answer(row(i));
        for (int j = 1; j < cols_; ++j)
            answer_[0][j] = S.answer(col(j));
        for (int i = 1; i < rows_; ++i)
            for (int j = 1; j < cols_; ++j)
                answer_[i][j] = square_[i][j] - answer_[i - 1][j] - answer_[i][j - 1] - answer_[i - 1][j - 1];
        return true;
    }

    int rows_, cols_;
    vector< vector<int> > square_;

    vector< vector<bool> > answer_;
    bool magicWorked = false;
};

int main() {
    ifstream cin("aladdin.in");
    ofstream cout("aladdin.out");

    int N, M; cin >> N >> M;

    Aladdin A(N, M);
    for (int i = 1; i < N; ++i)
        for (int j = 1; j < M; ++j) {
            int x; cin >> x;
            A.set(i, j, x);
        }

    try {
        A.magic();
        // yey the magic worked
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < M; ++j)
                cout << A[i][j] << " ";
            cout << "\n";
        }
    } catch (const NoMagicException &e) {
        cout << "-1\n";
    }
}
