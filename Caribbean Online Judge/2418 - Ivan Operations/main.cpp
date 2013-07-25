#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <fstream>

using namespace std;

class Treap {
  public:
    Treap(const int &value, Treap* const left = 0, Treap* const right = 0, const bool &nil = false);

    void update() {
        if (value_ == -1)
            return;
        if (reversed_) {
            swap(left_, right_);
            left_ -> reversed_ ^= true;
            right_ -> reversed_ ^= true;
            reversed_ = false;
        }

        if (lazy_ != -1) {
            value_ = lazy_;
            left_ -> lazy_ = right_ -> lazy_ = lazy_;
            memset(left_->count_, 0, sizeof(left_->count_));
            memset(right_->count_, 0, sizeof(right_->count_));
            left_->count_[lazy_] = left_->total_;
            right_->count_[lazy_] = right_->total_;
            lazy_ = -1;
        }

        total_ = left_ -> total_ + right_ -> total_ + 1;
        for (int i = 0; i < 26; ++i)
            count_[i] = left_ -> count_[i] + right_ -> count_[i];
        count_[value_] += 1;
    }

    int count(int value, int where) {
        update();

        if (where == 0)
            return 0;
        if (where <= left_ -> total_)
            return left_ -> count(value, where);
        int answer = left_ -> count_[value];
        where -= left_ -> total_;
        answer += (value_ == value);
        if (where == 1)
            return answer;
        --where;
        return answer + right_ -> count(value, where);
    }

    void set(int value) {
        lazy_ = value;
        update();
    }

    void reverse() {
        reversed_ ^= true;
        update();
    }

  private:
    friend Treap* merge(Treap *, Treap*);
    friend pair<Treap*, Treap*> split(Treap*, int);

    int value_;
    int priority_;
    Treap *left_, *right_;
    int total_;
    int count_[26];
    bool reversed_;
    int lazy_;
};

Treap* NIL = new Treap(0, 0, 0, true);

Treap::Treap(const int &value, Treap * const left, Treap * const right, const bool &nil) {
    if (nil) {
        value_ = -1;
        priority_ = -1;
        total_ = 0;
        memset(count_, 0, sizeof(count_));
        left_ = right_ = this;
    } else {
        value_ = value;
        priority_ = rand();
        if (left)
            left_ = left;
        else
            left_ = NIL;
        if (right)
            right_ = right;
        else
            right_ = NIL;
        memset(count_, 0, sizeof(count_));
        total_ = 0;
        count_[value_] = 1;
        for (int i = 0; i < 26; ++i) {
            count_[i] += left_->count_[i] + right_->count_[i];
            total_ += count_[i];
        }
    }
    reversed_ = false;
    lazy_ = -1;
}

Treap* merge(Treap* first, Treap* second) {
    if (first == NIL and second == NIL)
        return NIL;

    first -> update();
    second -> update();

    if (first -> priority_ > second -> priority_) {
        first -> right_ = merge(first -> right_, second);
        first -> update();
        return first;
    }

    second -> left_ = merge(first, second -> left_);
    second -> update();
    return second;
}

pair<Treap*, Treap*> split(Treap* current, int where) {
    if (current == NIL)
        return make_pair(NIL, NIL);

    if (where == 0)
        return make_pair(NIL, current);

    current -> update();
    if (where <= current -> left_ -> total_) {
        pair<Treap*, Treap*> now = split(current -> left_, where);
        current -> left_ = now.second;
        current -> update();

        return make_pair(now.first, current);
    }

    where -= current -> left_ -> total_ + 1;
    pair<Treap*, Treap*> now = split(current -> right_, where);
    current -> right_ = now.first;
    current -> update();

    return make_pair(current, now.second);
}

int main() {
    srand(time(NULL));

    string S; cin >> S;

    vector<int> value(S.size());
    for (int i = 0; i < int(S.size()); ++i)
        value[i] = S[i] - 'a';

    Treap *T = new Treap(value[0]);
    for (int i = 1; i < int(value.size()); ++i)
        T = merge(T, new Treap(value[i]));

    // we have the treap now

    int N; cin >> N;
    for (int i = 0; i < N; ++i) {
        char type; cin >> type;

        if (type == 'C') {
            int a, b; cin >> a >> b;
            char character; cin >> character;
            character -= 'a';

            cout << T -> count(character, b) - T -> count(character, a - 1) << "\n";
            continue;
        }

        if (type == 'S') {
            int a, b; cin >> a >> b;
            char character; cin >> character;
            character -= 'a';

            pair<Treap*, Treap*> p123 = split(T, b);
            pair<Treap*, Treap*> p12 = split(p123.first, a - 1);

            p12.second -> set(character);

            T = merge(p12.first, p12.second);
            T = merge(T, p123.second);
            continue;
        }

        int a, b; cin >> a >> b;

        pair<Treap*, Treap*> p123 = split(T, b);
        pair<Treap*, Treap*> p12 = split(p123.first, a - 1);

        p12.second -> reverse();
        T = merge(p12.first, p12.second);
        T = merge(T, p123.second);
    }
}
