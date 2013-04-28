#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>

using namespace std;

class HashMap {
  public:
    HashMap(const size_t &bits = 4) {
        initialize(1 << bits);
    }

    template<class InputIterator>
    HashMap(InputIterator first, InputIterator last) {
        size_t elements = distance(first, last);
        int size = 1;
        int maximum_capacity = kLoadFactor * size;
        while (maximum_capacity < elements) {
            size *= 2;
            maximum_capacity = kLoadFactor * size;
        }

        initialize(size);
        insert(first, last);
    }

    ~HashMap() {
        delete[] data_;
        delete[] mapa_;
    }

    void clear() {
        for (size_t i = 0; i < size_; ++i)
            data_[i] = -1;
        non_empty_elements_ = 0;
        filled_elements_ = 0;
    }

    class iterator : public std::iterator<std::input_iterator_tag, int> {
      public:
        iterator(int* const data, int* const mapa, int* const data_end):
          data_(data),
          mapa_(mapa),
          data_end_(data_end) {
        }

        iterator(const iterator& that):
          data_(that.data_),
          mapa_(that.mapa_),
          data_end_(that.data_end_) {
        }

        bool empty() const {
            return data_ != data_end_ && *data_ == -1;
        }

        bool filled() const {
            return data_ == data_end_ || *data_ >= 0;
        }

        iterator& operator=(const pair<int, int> &that) {
            *data_ = that.first;
            *mapa_ = that.second;
            return *this;
        }

        bool operator!=(const iterator& that) const {
            return data_ != that.data_ || mapa_ != that.mapa_;
        }

        bool operator!=(const int& that) const {
            return *data_ != that;
        }

        bool operator==(const int& that) const {
            return *data_ == that;
        }

        bool operator==(const iterator& that) const {
            return data_ == that.data_ && mapa_ == that.mapa_;
        }

        const pair<int, int> operator*() const {
            return make_pair(*data_, *mapa_);
        }

        iterator& operator++() {
            do {
                data_++;
                mapa_++;
            } while (!filled());

            return *this;
        }

      private:
        friend class HashMap;

        int *data_;
        int *mapa_;
        const int *data_end_;
    };

    iterator find(const int& data) {
        iterator it = pointTo(data);
        while (!it.empty() && it != data) {
            it = pointToNext(it);
        }

        if (it.empty())
            return end();
        return it;
    }

    int& operator[](const int &data) {
        iterator it = find(data);
        if (it == end())
            it = insert(data, 0);
        return *(it.mapa_);
    }

    iterator insert(const int& element, const int &value) {
        iterator it = find(element);
        if (it != end())
            return it;
        it = pointTo(element);
        while (it.filled()) {
            it = pointToNext(it);
        }

        if (it.empty())
            ++non_empty_elements_;

        it = make_pair(element, value);
        ++filled_elements_;

        if (non_empty_elements_ == maximum_capacity_) {
            grow();
            return find(element);
        }
        return it;
    }

    template<class InputIterator>
    void insert(InputIterator first, InputIterator last) {
        while (first != last) {
            insert((*first).first, (*first).second);
            ++first;
        }
    }

    bool erase(const int& element) {
        iterator it = find(element);

        if (it != end()) {
            erase(it);
            return true;
        }

        return false;
    }

    bool erase(const iterator& iterator) {
        *(iterator.data_)  = -2;

        filled_elements_--;

        return true;
    }

    iterator begin() {
        iterator it(data_, mapa_, data_ + size_);
        if (!it.filled())
            ++it;
        return it;
    }

    iterator end() {
        return iterator(data_ + size_, mapa_ + size_, data_ + size_);
    }

    size_t size() const {
        return filled_elements_;
    }

  private:
    static const double kLoadFactor; 

    void initialize(const int &size) {
        size_ = size;
        maximum_capacity_ = kLoadFactor * size;
        non_empty_elements_ = 0;
        filled_elements_ = 0;

        data_ = new int[size];
        mapa_ = new int[size];
        for (int i = 0; i < size; ++i)
            data_[i] = -1;
    }

    iterator pointToNext(const iterator &current) {
        size_t position = (current.data_ - data_ + 1) & (size_ - 1);
        return iterator(data_ + position, mapa_ + position, data_ + size_);
    }

    iterator pointTo(const int& data) {
        size_t position = (data * 41) & (size_ - 1);
        return iterator(data_ + position, mapa_ + position, data_ + size_);
    }

    void grow() {
        int *old_data = data_;
        int* old_mapa = mapa_;
        iterator old_begin = begin(), old_end = end();

        initialize(size_ * 2);

        insert(old_begin, old_end);

        delete[] old_data;
        delete[] old_mapa;
    }

    size_t size_;
    size_t maximum_capacity_;
    size_t non_empty_elements_;
    size_t filled_elements_;

    int *data_;
    int *mapa_;
};

const double HashMap::kLoadFactor = 0.7;

long long answer = 0;

const int kMaxN = (1 << 20) + 10;

HashMap from[kMaxN];
HashMap many[kMaxN];
int zero[kMaxN];

void build(int nod, int left, int right) {
    many[nod][0] = right - left + 1;
    //cerr << " -------> " << many[nod].size() << " " << many[nod][0] << "\n";
    if (left == right)
        return;
    int mid = (left + right) / 2;
    build(nod * 2, left, mid);
    build(nod * 2 + 1, mid + 1, right);
}

int get(HashMap &many, int c) {
    auto it = many.find(c);
    if (it == many.end())
        return 0;
    return (*it).second;
}

void turn(int nod, int oldc, int newc, bool to_add = false) {
    //cerr << "turn in " << nod << " " << oldc << " in " << newc << "\n";
    int x = get(many[nod], oldc);
    if (!x)
        return;
    if (to_add)
        answer += x;
    //cerr << "now ->\n";
    //for (auto & p : many[nod])
    //    cerr << p.first << " " << p.second << "\n";

    //cerr << "from ->\n";
    //for (auto & p : from[nod])
    //    cerr << p.second << " " << p.first << "\n";

    //if (zero[nod])
     //   cerr << 0 << " " << zero[nod] << "\n";

   //cerr << "working\n";
    many[nod][newc] += x;
    many[nod].erase(oldc);
    if (oldc == 0 || zero[nod] == oldc)
        zero[nod] = newc;
    if (oldc) {
        if (get(from[nod], oldc)) {
            from[nod][newc] = from[nod][oldc];
            from[nod].erase(oldc);
        } else
            from[nod][newc] = oldc;
    }

    //cerr << "now ->\n";
    /*for (auto & p : many[nod])
        cerr << p.first << " " << p.second << "\n";

    cerr << "from ->\n";
    for (auto & p : from[nod])
        cerr << p.second << " " << p.first << "\n";

    if (zero[nod])
        cerr << 0 << " " << zero[nod] << "\n";

    cerr << "\n\n";*/
}

void lazy_update(int nod) {
    //cerr << "lazy in " << nod << "\n";
    if (!from[nod].size() && !zero[nod])
        return;

    for (auto &how : from[nod]) {
        int tmp_oldc = how.second;
        int tmp_newc = how.first;
        turn(nod * 2, tmp_oldc, tmp_newc);
        turn(nod * 2 + 1, tmp_oldc, tmp_newc);
        //cerr << tmp_oldc << " -> " << tmp_newc << "\n";
    }

    from[nod].clear();
    if (zero[nod]) {
        turn(nod * 2, 0, zero[nod]);
        turn(nod * 2 + 1, 0, zero[nod]);
        //cerr << 0 << " -> " << zero[nod] << "\n";
        zero[nod] = 0;
    }
}

void update(int nod, int left, int right, int start, int finish, int oldc, int newc) {
    if (start <= left && right <= finish) {
        turn(nod, oldc, newc, true);
        return;
    }

    lazy_update(nod);  // we just send to childs
    int mid = (left + right) / 2;
    if (start <= mid)
        update(nod * 2, left, mid, start, finish, oldc, newc);
    if (mid < finish)
        update(nod * 2 + 1, mid + 1, right, start, finish, oldc, newc);

    many[nod][oldc] = get(many[nod * 2], oldc) + get(many[nod * 2 + 1], oldc);
    many[nod][newc] = get(many[nod * 2], newc) + get(many[nod * 2 + 1], newc);

    auto it = many[nod].find(oldc);
    if ((*it).second == 0)
        many[nod].erase(it);
    it = many[nod].find(newc);
    if ((*it).second == 0)
        many[nod].erase(it);
    //cerr << "-----------------------------------------------------------------------------\n";
    //cerr << nod << " -> " << " In the interval " << left << " " << right << " we have " << get(many[nod], oldc) << " of " << oldc << " and " << get(many[nod], newc) << " of " << newc << "\n";
    //cerr << "-----------------------------------------------------------------------------\n";
}

int query(int nod, int left, int right, int where) {
    if (left == right) {
        return (*many[nod].begin()).first;
    }

    lazy_update(nod);
    int mid = (left + right) / 2;
    if (where <= mid)
        return query(nod * 2, left, mid, where);
    else
        return query(nod * 2 + 1, mid + 1, right, where);
}

int main() {
    ifstream cin("input.txt");
    ofstream cout("output.txt");

    int N, Q; cin >> N >> Q;

    build(1, 1, N);

    for (int i = 1; i <= Q; ++i) {
        //cerr << "\n\n\n\n\n\n";
        int x, y; cin >> x >> y;

        int this_colour = query(1, 1, N, x);
        if (this_colour)
            update(1, 1, N, x, y, this_colour, i);

        update(1, 1, N, x, y, 0, i);

        //cerr << " aiiiiiiiici " << answer << "\n";
    }

    cout << answer << "\n";
}
