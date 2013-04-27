/**
 * HashSet using open addressing (also known as closed hashing) to hash values
 * The keys can be of any type hashed by std::tr1::hash class template
 * For any other types the hash class template must be specialized
 * The load factor is best put at 0.8 and the starting size to 16 elements (only the second value
 * can be modified)
 *
 * It is a growing hash. That means that when it hits its load factor it doubles its capacity.
 * Code might be extended in the future to use move-semantics (C++-11)
 * You can also iterate on the hash using the InputIterator pair begin() and end()
 *
 * Overall very similar to the stl-unordered_set yet faster (30% faster on the growing hash and 10% faster if the
 * size is known in advance) and using less memory (up to 30% less memory)
 */
#include <iostream>
#include <fstream>
#include <algorithm>
#include <tr1/functional>

template<class DataType>
class HashSet {
  public:
    HashSet(const size_t &bits = 4) {
        initialize(1 << bits);
    }

    ~HashSet() {
        delete[] data_;
        delete[] data_type_;
    }

    class iterator : public std::iterator<std::input_iterator_tag, DataType> {
      public:
        iterator(DataType* const data, char* const data_type, DataType* const data_end):
          data_(data),
          data_type_(data_type),
          data_end_(data_end) {
        }

        iterator(const iterator& that):
          data_(that.data_),
          data_type_(that.data_type_),
          data_end_(that.data_end_) {
        }

        bool empty() const {
            return data_ != data_end_ && *data_type_ == kEmptyElement;
        }

        bool filled() const {
            return data_ == data_end_ || *data_type_ == kFilledElement;
        }

        iterator& operator=(const DataType& that) {
            *data_ = that;
            *data_type_ = kFilledElement;
            return *this;
        }

        bool operator!=(const iterator& that) const {
            return data_ != that.data_ || data_type_ != that.data_type_;
        }

        bool operator!=(const DataType& that) const {
            return *data_ != that;
        }

        bool operator==(const DataType& that) const {
            return *data_ == that;
        }

        bool operator==(const iterator& that) const {
            return data_ == that.data_ && data_type_ == that.data_type_;
        }

        const DataType& operator*() const {
            return *data_;
        }

        iterator& operator++() {
            do {
                data_++;
                data_type_++;
            } while (!filled());

            return *this;
        }

      private:
        enum {
            kEmptyElement = 0,
            kErasedElement = 1,
            kFilledElement = 2,
        };
       
        friend class HashSet;

        DataType *data_;
        char *data_type_;
        const DataType *data_end_;
    };

    iterator find(const DataType& data) {
        iterator it = pointTo(data);
        while (!it.empty() && it != data) {
            it = pointToNext(it);
        }

        if (it.empty())
            return end();
        return it;
    }


    std::pair<iterator, bool> insert(const DataType& element) {
        iterator it = find(element);
        if (it != end())
            return std::pair<iterator, bool>(it, false);
        it = pointTo(element);
        while (it.filled()) {
            it = pointToNext(it);
        }

        if (it.empty())
            ++non_empty_elements_;

        it = element;

        if (non_empty_elements_ == maximum_capacity_) {
            grow();
            return std::pair<iterator, bool>(find(element), true);
        }

        return std::pair<iterator, bool>(it, true);
    }

    template<class InputIterator>
    size_t insert(InputIterator first, InputIterator last) {
        size_t inserted = 0;
        while (first != last) {
            if (insert(*first).second)
                ++inserted;
            ++first;
        }

        return inserted;
    }

    bool erase(const DataType& element) {
        iterator it = find(element);

        if (it != end()) {
            erase(it);
            return true;
        }

        return false;
    }

    bool erase(const iterator& iterator) {
        *(iterator.data_)  = DataType();
        *(iterator.data_type_) = kErasedElement;

        filled_elements_--;

        return true;
    }

    iterator begin() {
        iterator it(data_, data_type_, data_ + size_);
        if (!it.filled())
            ++it;
        return it;
    }

    iterator end() {
        return iterator(data_ + size_, data_type_ + size_, data_ + size_);
    }

    size_t size() const {
        return filled_elements_;
    }

  private:
    enum {
        kEmptyElement = 0,
        kErasedElement = 1,
        kFilledElement = 2,
    };

    static const double kLoadFactor; 

    void initialize(const int &size) {
        size_ = size;
        maximum_capacity_ = kLoadFactor * size;
        non_empty_elements_ = 0;
        filled_elements_ = 0;

        data_ = new DataType[size];
        data_type_ = new char[size];
        for (int i = 0; i < size; ++i)
            data_type_[i] = kEmptyElement;
    }

    iterator pointToNext(const iterator &current) {
        size_t position = (current.data_ - data_ + 1) & (size_ - 1);
        return iterator(data_ + position, data_type_ + position, data_ + size_);
    }

    iterator pointTo(const DataType& data) {
        size_t position = (hash_function(data) * 41) & (size_ - 1);
        return iterator(data_ + position, data_type_ + position, data_ + size_);
    }

    void grow() {
        DataType *old_data = data_;
        char* old_data_type = data_type_;
        iterator old_begin = begin(), old_end = end();

        initialize(size_ * 2);

        insert(old_begin, old_end);

        delete[] old_data;
        delete[] old_data_type;
    }

    size_t size_;
    size_t maximum_capacity_;
    size_t non_empty_elements_;
    size_t filled_elements_;

    DataType *data_;

    // FIXME: we are now using 8 bits for information for which we only need 2 bits, optimize a little
    char *data_type_;

    // Extra memory for some speed (maybe?)
    std::tr1::hash<DataType> hash_function;
};

template<class DataType>
const double HashSet<DataType>::kLoadFactor = 0.8;

using namespace std;

int main() {
    ifstream cin("hashuri.in");
    ofstream cout("hashuri.out");

    int N; cin >> N;
    HashSet<int> H;

    for (int i = 0; i < N; ++i) {
        int type, value;
        cin >> type>> value;
        if (type == 1)
            H.insert(value);
        if (type == 2)
            H.erase(value);
        if (type == 3)
            cout << bool(H.find(value) != H.end()) << "\n";
    }
}
