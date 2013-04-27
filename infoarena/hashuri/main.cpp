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
    struct element {
        DataType value;
        element *next;
    };


    HashSet(const size_t &size = 4) {
        initialize(1 << size);
    }

    template<class InputIterator>
    HashSet(InputIterator first, InputIterator last) {
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

    ~HashSet() {
        delete[] data_;
    }

    element** find(const DataType& data) {
        element* where = data_[pointTo(data)];

        while (where != 0 && where -> value != data)
            where = where -> next;
        return &where;
    }


    void insert(const DataType& element) {
        if (*find(element) != 0)
            return;
        _insert(element);
    }

    void erase(const DataType& data) {
        element** where = find(data);
        if (*where != 0) {
            element* now = *where;
            *where = now -> next;
            delete now;
        }
    }

    size_t size() const {
        return filled_elements_;
    }

    element* end() const {
        return 0;
    }

  private:
    static const double kLoadFactor; 

    size_t pointTo(const DataType& element) {
        return ((hash_function(element) ^ 0x9e3779b9) * 41) & (size - 1);
    }

    void initialize(const int &size) {
        size_ = size;
        filled_elements_ = 0;

        data_ = new element*[size];
        memset(data_, 0, sizeof(element*) * size);
    }

    void grow() {
        size_t old_size = size_;
        element **old_data = data_;

        initialize(size_ * 2);

        for (int i = 0; i < old_size; ++i)
            while (data_[i] != 0) {
                element* now = data_[i];
                insert(now -> value);
                data_[i] = now -> next;
                delete now;
            }

        delete[] old_data;
    }

    size_t size_;
    size_t filled_elements_;

    element **data_;
    std::tr1::hash<DataType> hash_function;
};

template<class DataType>
const double HashSet<DataType>::kLoadFactor = 1;

using namespace std;

int main() {
    ifstream cin("hashuri.in");
    ofstream cout("hashuri.out");

    HashSet<int> H;

    int N; cin >> N;
    for (int i = 0; i < N; ++i) {
        int type, value;
        cin >> type>> value;
        if (type == 1)
            H.insert(value);
        if (type == 2)
            H.erase(value);
        if (type == 3)
            cout << bool(*H.find(value) != 0) << "\n";
    }
}
