#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <cassert>
#include <cstring>

using namespace std;

int dx[4] = {-1, 0, 1, 0};
int dy[4] = {0, -1, 0, 1};
char ch[4] = {'N', 'V', 'S', 'E'};

class State {
  public:
    State() {
        data_ = new char[size_ * size_];
    }

    State(const State& that) {
        data_ = new char[size_ * size_];
        memcpy(data_, that.data_, size_ * size_ * sizeof(char));
    }

    State& operator=(const State& that) {
        data_ = that.data_;
        return *this;
    }

    char* operator[](int index) {
        return data_ + index * size_;
    }

    static void setSize(int newSize) {
        size_ = newSize;
    }

    bool operator!=(const State& that) const {
        for (int i = 0; i < size_ * size_; ++i)
            if (data_[i] != that.data_[i])
                return true;
        return false;
    }

    bool operator==(const State& that) const {
        return !(*this != that);
    }

  private:
    friend class hash<State>;

    char *data_;
    static int size_;
};

int State::size_ = 1;

namespace std {

template<>
class hash<State> {
  public:
    size_t operator()(const State& object) const {
        return hasher_(string(object.data_, State::size_ * State::size_));
    };

  private:
    hash<string> hasher_;
};

}

int main() {
    ifstream cin("parcare.in");
    ofstream cout("parcare.out");

    int N; cin >> N;
    vector<string> V(N);
    for (int i = 0; i < N; ++i)
        cin >> V[i];

    State::setSize(N);
    State start;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            start[i][j] = V[i][j];

    queue<State> Q;
    Q.push(start);

    State end;
    unordered_map<State, pair<State, string>> move;
    move[start] = {start, "nothing"};

    while (!Q.empty()) {
        auto now = Q.front();
        Q.pop();

        for (int j = 0; j < N; ++j)
            if (now[0][j] == 'A') {
                end = now;
                break;
            }

        if (end[0][0] == '#')
            break;

        for (int k = 0; k < 4; ++k)
            for (int i = 1; i < N - 1; ++i)
                for (int j = 1; j < N - 1; ++j)
                    if (now[i][j] != '.' and now[i][j] != '#' and now[i][j] == now[i - dx[k]][j - dy[k]] and now[i + dx[k]][j + dy[k]] == '.') { // yey car
                        int x = i;
                        int y = j;
                        auto next = now;
                        while (next[x][y] == now[i][j]) {
                            next[x + dx[k]][y + dy[k]] = now[i][j];
                            next[x][y] = '.';
                            x -= dx[k];
                            y -= dy[k];
                        }

                        string from;
                        from += now[i][j];
                        from += ch[k];
                        from += '1';

                        decltype(move.begin()) it;
                        bool new_element;
                        tie(it, new_element) = move.emplace(next, make_pair(now, from));
                        if (new_element)
                            Q.push(next);
                    }
    }

    vector<string> answer;
    for (auto it = end; it != start; it = move[it].first)
        answer.push_back(move[it].second);

    reverse(answer.begin(), answer.end());
    answer.pop_back();
    answer.push_back("Exit");
    cout << answer.size() << "\n";
    for (auto &s : answer)
        cout << s << "\n";
}
