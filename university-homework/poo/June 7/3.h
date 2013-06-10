#include "1.h"
#include "2.h"

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
