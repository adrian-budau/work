/**
 * Adrian Budau, Grupa 134
 */



// Matricile sunt indexate de la 0
// Se arunca exceptii, aveti grija la Killed by Signal 6
#include <exception>
#include <iostream>

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
        delete[] data_;
    }

    Matrix& operator=(const Matrix& that) {
        delete[] data_;

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

class SMatrix : public Matrix<Z7> {
  public:
    SMatrix():
            Matrix(2, 2, 0) {
    }

    SMatrix(const Matrix<Z7> &that):
            Matrix(that) {
    }

    SMatrix(const SMatrix& that):
            Matrix(that) {
    }

    Z7 determinant() const {
        return (*this)[0][0] * (*this)[1][1] - (*this)[0][1] * (*this)[1][0];
    }
};

int main() {
    // Operatii cu Z7, se arunca exceptii daca se cere impartire la 0 sau inversul lui 0
    int x1 = 3, y1 = 4;
    Z7 a = x1, b = y1;
    cout << a << " " << b << " " << a + b << " " << a - b << " " << a * b << " " << a / b << "\n";

    cout << "\n\n\n";
    // Operatii cu Matrici, se arunca exceptii daca se inmultesc 2 matrici care nu se potrivesc ca dimensiuni sau se aduna 2 matrici care nu se potrivesc ca dimensiuni
    Matrix<int> M(5, 5, 8);

    Matrix<int> P(5, 5, 5);

    Matrix<int> V = M * P;

    for (int i = 0; i < V.rows(); ++i) {
        for (int j = 0; j < V.columns(); ++j)
            cout << V[i][j] << " ";
        cout << "\n";
    }

    Matrix<int> U = M + P;
    for (int i = 0; i < U.rows(); ++i) {
        for (int j = 0; j < U.columns(); ++j)
            cout << U[i][j] << " ";
        cout << "\n";
    }
    cout << "\n\n\n";

    // Operatii cu Matrici de 2x2 in Z7
    SMatrix A;
    A[0][0] = 1;
    A[0][1] = 4;
    A[1][0] = 2;
    A[1][1] = 6;

    SMatrix B = A + A;
    SMatrix C = A * B;

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j)
            cout << A[i][j] << " ";
        cout << "\n";
    }

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j)
            cout << B[i][j] << " ";
        cout << "\n";
    }

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j)
            cout << C[i][j] << " ";
        cout << "\n";
    }

    cout << C.determinant() << "\n";
}
