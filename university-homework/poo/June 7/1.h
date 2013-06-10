#include <exception>

using namespace std;

class Z7 {
  public:
    Z7(const int &data = 0):
            data_(data % 7) {
        if (data_ < 0)
            data_ += 7;
    }

    Z7 operator+(const Z7 &that) const {
        return Z7(data_ + that.data_);
    }

    Z7 operator-(const Z7 &that) const {
        return Z7(data_ - that.data_);
    }

    Z7 operator*(const Z7 &that) const {
        return Z7(data_ * that.data_);
    }

    Z7 operator/(const Z7 &that) const {
        return *this * that.inverse();
    }

    Z7 inverse() const {
        if (data_ == 0)
            throw new exception();
        int answer = 1;
        for (int i = 0; i < 5; ++i)
            answer = (answer * data_) % 7;
        return answer;
    }

    Z7& operator+=(const Z7 &that) {
        *this = *this + that;
        return *this;
    }

    Z7& operator*=(const Z7 &that) {
        *this = *this * that;
        return *this;
    }

    operator int() {
        return data_;
    }

  private:
    int data_;
};


