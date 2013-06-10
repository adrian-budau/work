#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <limits>
#include <set>

using namespace std;

const int kInfinite = numeric_limits<int>::max();

struct Operation {
    enum {
        INSERT_VALUE = 1,
        INSERT_DELETE = 2,
        DELETE_INSERT = 3,
        DELETE_DELETE = 4,
        QUERY = 5
    };

    int  type, time, value;
};

istream& operator>>(istream& stream, Operation& object) {
    stream >> object.type;

    switch (object.type) {
        case Operation::INSERT_VALUE:
            stream >> object.value;
            break;
        case Operation::INSERT_DELETE:
            stream >> object.time >> object.value;
            break;
        case Operation::DELETE_INSERT:
            stream >> object.value;
            break;
        case Operation::DELETE_DELETE:
            stream >> object.time >> object.value;
            break;
        default:
            stream >> object.time >> object.value;
    }

    return stream;
}

class SegmentTree {
  public:
    explicit SegmentTree(const int &size) {
        for (size_ = 1; size_ < size; size_ *= 2);
        deletes_.resize(size);
        when_late_.resize(size_ * 2, kInfinite);
    }

    void addDelete(const int &value, const int &time) {
        deletes_[value].insert(time);
        update(value);
    }

    void dropDelete(const int &value, const int &time) {
        deletes_[value].erase(deletes_[value].find(time));
        update(value);
    }

    int best(const int &value, const int &time) {
        return best(1, 0, size_, value, size_, time);
    }

    int size() const {
        return size_;
    }

  private:
    void update(int value) {
        if (deletes_[value].size())
            when_late_[size_ + value] = *deletes_[value].begin();
        else
            when_late_[size_ + value] = kInfinite;

        for (int position = (size_ + value) / 2; position; position /= 2)
            when_late_[position] = max(when_late_[position * 2], when_late_[position * 2 + 1]);
    }

    int best(const int &node, const int &begin, const int &end, const int &left, const int &right, const int &time) {
        if (when_late_[node] <= time)
            return kInfinite;

        if (begin + 1 == end)
            return begin;

        int mid = (begin + end) / 2;
        if (right < mid)
            return best(node * 2, begin, mid, left, right, time);
        if (mid <= left)
            return best(node * 2 + 1, mid, end, left, right, time);

        int where = best(node * 2, begin, mid, left, right, time);
        if (where != kInfinite)
            return where;
        return best(node * 2 + 1, mid, end, left, right, time);
    }

    int size_;
    vector<multiset<int>> deletes_;
    vector<int> when_late_;
};

int main() {
    ifstream cin("timetravel.in");
    ofstream cout("timetravel.out");

    int M; cin >> M;

    vector<Operation> V(M);

    vector<int> values;
    for (int i = 0; i < M; ++i) {
        cin >> V[i];
        if (V[i].type == Operation::INSERT_VALUE)
            values.push_back(V[i].value);
    } 

    sort(values.begin(), values.end());
    values.erase(unique(values.begin(), values.end()), values.end());

    SegmentTree S(values.size());
    for (size_t i = 0; i < values.size(); ++i)
        S.addDelete(i, -kInfinite);                 // time small enough

    for (int i = 0; i < M; ++i) {
        int x;
        switch (V[i].type) {
            case Operation::INSERT_VALUE:
                x = lower_bound(values.begin(), values.end(), V[i].value) - values.begin();
                S.dropDelete(x, -kInfinite);
                break;
            case Operation::INSERT_DELETE:
                x = lower_bound(values.begin(), values.end(), V[i].value) - values.begin();
                if (x != int(values.size()) and V[i].value == values[x])
                    S.addDelete(x, V[i].time);
                break;
            case Operation::DELETE_INSERT:
                x = lower_bound(values.begin(), values.end(), V[i].value) - values.begin();
                if (x != int(values.size()) and V[i].value == values[x])
                    S.addDelete(x, -kInfinite);
                break;
            case Operation::DELETE_DELETE:
                x = lower_bound(values.begin(), values.end(), V[i].value) - values.begin();
                if (x != int(values.size()))
                    S.dropDelete(x, V[i].time);
                break;
            case Operation::QUERY:
                x = lower_bound(values.begin(), values.end(), V[i].value) - values.begin();
                if (x == int(values.size())) {
                    cout << "Time paradox\n";
                    break;
                }
                int y = S.best(x, V[i].time);
                if (y >= int(values.size())) {
                    cout << "Time paradox\n";
                    break;
                }

                cout << values[y] << "\n";
        }
    }
}
