#include <exception>

using namespace std;

template<typename T>
class Matrix {
  public:
    Matrix(int rows, int columns, const T& value = T()):
            rows_(rows),
            columns_(columns) {
        data_ = new T[rows * columns];
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < columns; ++j)
                if (i == j)
                    data_[i * columns_ + j] = value;
                else
                    data_[i * columns_ + j] = T();          // which should yield 0 for int, floats, etc
    }

    Matrix(const Matrix& that):
            rows_(that.rows_),
            columns_(that.columns_) {
        data_ = new T[rows_ * columns_];
        for (int i = 0; i < rows_; ++i)
            for (int j = 0; j < columns_; ++j)
                data_[i * columns_ + j] = that[i][j];
    }

    ~Matrix() {
        delete data_;
    }

    Matrix& operator=(const Matrix& that) {
        delete data_;

        rows_ = that.rows_;
        columns_ = that.columns_;
        data_ = new T[rows_ * columns_];
        for (int i = 0; i < rows_; ++i)
            for (int j = 0; j < columns_; ++j)
                data_[i * columns + j] = that[i][j];
        return *this;
    }

    Matrix operator+(const Matrix& that) const {
        if (rows_ != that.rows_ || columns_ != that.columns_)
            throw new exception();

        Matrix<T> result(*this);
        for (int i = 0; i < rows_; ++i)
            for (int j = 0; j < columns_; ++j)
                result[i][j] += that[i][j];
        return result;
    }

    Matrix operator*(const Matrix& that) const {
        if (columns_ != that.rows_)
            throw new exception();

        Matrix<T> result(rows(), that.columns(), T());

        for (int i = 0; i < rows(); ++i)
            for (int j = 0; j < columns(); ++j)
                for (int k = 0; k < that.columns(); ++k)
                    result[i][k] += (*this)[i][j] * that[j][k];
        return result;
    }

    int rows() const {
        return rows_;
    }

    int columns() const {
        return columns_;
    }

    T* operator[](const int &row) {
        return data_ + row * columns_;
    }

    const T* operator[](const int &row) const {
        return data_ + row * columns_;
    }

  private:
    int rows_, columns_;
    T* data_;
};
