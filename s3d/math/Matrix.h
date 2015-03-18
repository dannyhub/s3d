#ifndef S3D_MATH_MATRIX_H
#define S3D_MATH_MATRIX_H

#include <initializer_list>
#include <cassert>
#include <algorithm>
#include <stdexcept>

namespace s3d
{

#define CHECK_THROW(con,except)  {assert(con); if (!(con)) throw (except);}

class MatrixInverseException : public std::exception {
public:
  MatrixInverseException(const char * const & msg) : exception(msg) {
  }
};

template<typename T, unsigned int Rows, unsigned int Cols>
class Matrix {
public:
  typedef T value_type;

public:
  Matrix() {
    for (unsigned int i = 0; i < Rows; ++i) {
      std::fill_n(m_[i], Cols, T(0));
    }
  }

  explicit Matrix(const T m[Rows][Cols]) {
    for (unsigned int r = 0; r < Rows; ++r) {
      for (unsigned int c = 0; c < Cols; ++c) {
        m_[r][c] = m[r][c];
      }
    }
  }

  Matrix(const std::initializer_list<T>& ilist) {
    assert(ilist.size() == (sizeof m_) / sizeof (T));

    auto bit = ilist.begin();
    for (unsigned int i = 0; i < Rows; ++i) {
      std::copy(bit, bit + Cols, m_[i]);
      bit += Cols;
    }
  }

  Matrix(const Matrix& m) : Matrix(m.m_) {
  }

  Matrix& operator= (const Matrix& m) {
    for (unsigned int r = 0; r < Rows; ++r) {
      for (unsigned int c = 0; c < Cols; ++c) {
        m_[r][c] = m[r][c];
      }
    }

    return *this;
  }

  bool isIdentify() const;
  T det() const;
  Matrix inverse() const;
  Matrix transpose() const;
  Matrix adjoint() const;

  inline T* operator[] (unsigned int row) {
    return m_[row];
  }

  inline const T* operator[] (unsigned int row) const {
    return m_[row];
  }

  T& at(unsigned int row, unsigned int col) {
    CHECK_THROW(row <= Rows && col <= Cols, std::out_of_range("Matrix at out_of_range"));
    return m_[row][col];
  }

  const T at(unsigned int row, unsigned int col) const {
    CHECK_THROW(row <= Rows && col <= Cols, std::out_of_range("Matrix at out_of_range"));
    return m_[row][col];
  }

private:
  T m_[Rows][Cols];
};

namespace matrix_impl
{

  inline bool equalZero(int a) {
    return a == 0;
  }

  inline bool equalZero(float a) {
    return ::fabs(a) < 1E-5;
  }

  inline bool equalZero(double a) {
    return ::fabs(a) < 1E-13;
  }

}// matrix_impl

template<typename T, unsigned int Rows, unsigned int Cols>
bool operator == (const Matrix<T, Rows, Cols>& m1, const Matrix<T, Rows, Cols>& m2) {
  for (unsigned int r = 0; r < Rows; ++r) {
    for (unsigned int c = 0; c < Cols; ++c) {
      if (!matrix_impl::equalZero(m1[r][c] - m2[r][c])) {
        return false;
      }
    }
  }

  return true;
}

template<typename T, unsigned int Rows, unsigned int Cols>
inline bool operator != (const Matrix<T, Rows, Cols>& m1, const Matrix<T, Rows, Cols>& m2) {
  return !(m1 == m2);
}

template<typename T, unsigned int Rows, unsigned int Cols>
Matrix<T, Rows, Cols> operator + (const Matrix<T, Rows, Cols>& m1, const Matrix<T, Rows, Cols>& m2) {
  Matrix<T, Rows, Cols> rm;
  for (unsigned int r = 0; r < Rows; ++r) {
    for (unsigned int c = 0; c < Cols; ++c) {
      rm[r][c] = m1[r][c] + m2[r][c];
    }
  }

  return rm;
}

template<typename T, unsigned int Rows, unsigned int Cols>
Matrix<T, Rows, Cols>& operator += (Matrix<T, Rows, Cols>& m1, const Matrix<T, Rows, Cols>& m2) {
  for (unsigned int r = 0; r < Rows; ++r) {
    for (unsigned int c = 0; c < Cols; ++c) {
      m1[r][c] = m1[r][c] + m2[r][c];
    }
  }

  return m1;
}

template<typename T, unsigned int Rows, unsigned int Cols>
Matrix<T, Rows, Cols> operator - (const Matrix<T, Rows, Cols>& m1, const Matrix<T, Rows, Cols>& m2) {
  Matrix<T, Rows, Cols> rm;
  for (unsigned int r = 0; r < Rows; ++r) {
    for (unsigned int c = 0; c < Cols; ++c) {
      rm[r][c] = m1[r][c] - m2[r][c];
    }
  }

  return rm;
}

template<typename T, unsigned int Rows, unsigned int Cols>
Matrix<T, Rows, Cols>& operator -= (Matrix<T, Rows, Cols>& m1, const Matrix<T, Rows, Cols>& m2) {
  for (unsigned int r = 0; r < Rows; ++r) {
    for (unsigned int c = 0; c < Cols; ++c) {
      m1[r][c] = m1[r][c] - m2[r][c];
    }
  }

  return m1;
}

template<typename T, unsigned int Rows, unsigned int Cols, typename ValueT>
Matrix<T, Rows, Cols> operator * (const Matrix<T, Rows, Cols>& m1, ValueT value) {
  Matrix<T, Rows, Cols> rm;
  for (unsigned int r = 0; r < Rows; ++r) {
    for (unsigned int c = 0; c < Cols; ++c) {
      rm[r][c] = m1[r][c] * value;
    }
  }

  return rm;
}

template<typename T, unsigned int Rows, unsigned int Cols, typename ValueT>
Matrix<T, Rows, Cols>& operator *= (Matrix<T, Rows, Cols>& m1, ValueT value) {
  for (unsigned int r = 0; r < Rows; ++r) {
    for (unsigned int c = 0; c < Cols; ++c) {
      m1[r][c] = m1[r][c] * value;
    }
  }

  return m1;
}

template<typename T, unsigned int Rows, unsigned int M, unsigned int Cols>
Matrix<T, Rows, Cols> operator * (const Matrix<T, Rows, M>& m1, const Matrix<T, M, Cols>& m2) {
  Matrix<T, Rows, Cols> rm;
  for (unsigned int r = 0; r < Rows; ++r) {
    for (unsigned int c = 0; c < Cols; ++c) {
      T sum = 0;
      for (unsigned int i = 0; i < M; ++i) {
        sum += m1[r][i] * m2[i][c];
       }

      rm[r][c] = sum;
    }
  }

  return rm;
}

template<typename T, unsigned int Rows, unsigned int Cols>
inline Matrix<T, Rows, Cols>& operator *= (Matrix<T, Rows, Cols>& m1, const Matrix<T, Rows, Cols>& m2) {
  m1 = m1 * M2;
  return m1;
}

template<typename T>
T matrixDet (const Matrix<T, 2, 2>& m1) {
  return m1[0][0] * m1[1][1] - m1[0][1] * m1[1][0];
}

template<typename T>
T matrixDet(const Matrix<T, 3, 3>& m1) {
  return m1[0][0] * m1[1][1] * m1[2][2] + m1[0][1] * m1[1][2] * m1[2][0] + m1[0][2] * m1[1][0] * m1[2][1]
        - m1[0][2] * m1[1][1] * m1[2][0] - m1[0][1] * m1[1][0] * m1[2][2] - m1[0][0] * m1[1][2] * m1[2][1];
}

namespace matrix_impl
{
  template<typename T>
  int computeInverseNumbers(T* array, unsigned size) {
    int sum = 0;
    for (unsigned i = 1; i < size; ++i) {
      if (array[i] < 0) {
        continue;
      }

      for (unsigned j = 0; j < i; ++j) {
        if (array[j] < 0) {
          continue;
        }
        if (array[i] < array[j]) {
          sum++;
        }
      }
    }
    return sum;
  }

  template<typename T, unsigned int M>
  T computeDet(const Matrix<T, M, M>& m1) {
    bool flags[M] = {0};
    int inverseNumbers[M] = {0};
    T sum = T(0);
    T colsum = T(1);
    computeDet<T, M>(m1, sum, colsum, 0, flags, inverseNumbers);
    return sum;
  }

  template<typename T, unsigned int M>
  T computeDetDetExclude(const Matrix<T, M, M>& m1, int excludeRow, int excludeCol) {
    bool flags[M] = {0};
    int inverseNumbers[M] = {0};
    T sum = T(0);
    T colsum = T(1);
    computeDet<T, M>(m1, sum, colsum, 0, flags, inverseNumbers, excludeRow, excludeCol);
    return sum;
  }

  template<typename T, unsigned int M>
  void computeDet(const Matrix<T, M, M>& m1, T& sum, T colsum, int currentRow, bool flags[M], int inverseNumbers[M]) {
    if (currentRow == M) {
      if (computeInverseNumbers(inverseNumbers, M) % 2 > 0) { 
        sum -= colsum;
      } else {
        sum += colsum;
      }
      
      return;
    }

    for (int i = 0; i < M; ++i) {
      if (flags[i]) {
        continue;
      }
      flags[i] = true;
      inverseNumbers[currentRow] = i + 1;
      const T tmpsum = colsum * m1[currentRow][i];
      computeDet<T, M>(m1, sum, tmpsum, currentRow + 1, flags, inverseNumbers);
      flags[i] = false;
    }
  }

  template<typename T, unsigned int M>
  void computeDetExclude(const Matrix<T, M, M>& m1, T& sum, T colsum, int currentRow, bool flags[M], int inverseNumbers[M], 
                         int excludeRow, int excludeCol) {
    if (currentRow == M || (excludeRow == M - 1) && (currentRow == excludeRow)) {
      if (computeInverseNumbers(inverseNumbers, M) % 2 > 0) {
        sum -= colsum;
      } else {
        sum += colsum;
      }

      return;
    }

    if (currentRow == excludeRow) {
      inverseNumbers[currentRow] = -1;
      computeDet<T, M>(m1, sum, colsum, currentRow + 1, flags, inverseNumbers, excludeRow, excludeCol);
      return;
    }

    for (int i = 0; i < M; ++i) {
      if (i == excludeCol) {
        continue;
      }
      if (flags[i]) {
        continue;
      }
      flags[i] = true;
      inverseNumbers[currentRow] = i + 1;
      const T tmpsum = colsum * m1[currentRow][i];
      computeDet<T, M>(m1, sum, tmpsum, currentRow + 1, flags, inverseNumbers, excludeRow, excludeCol);
      flags[i] = false;
    }
  }
}

template<typename T, unsigned int M>
inline T matrixDet(const Matrix<T, M, M>& m1) {
  return matrix_impl::computeDet<T, M>(m1);
}

template<typename T, unsigned int M>
inline Matrix<T, M, M> matrixTranspose(const Matrix<T, M, M>& m1) {
  Matrix<T, M, M> rm;
  for (unsigned int r = 0; r < M; ++r) {
    for (unsigned int c = 0; c < M; ++c) {
      rm[c][r] = m1[c][r];
    }
  }
  return rm;
}

template<typename T>
inline Matrix<T, 2U, 2U> matrixAdjoint(const Matrix<T, 2U, 2U>& m1) {
  Matrix<T, 2U, 2U> mat;
  mat[0][0] = m1[1][1];
  mat[0][1] = -m1[0][1];
  mat[1][0] = -m1[1][0];
  mat[1][1] = m1[0][0];
  return mat;
}

template<typename T>
Matrix<T, 3U, 3U> matrixAdjoint(const Matrix<T, 3U, 3U>& m1) {
  Matrix<T, 3U, 3U> mat;

  mat[0][0] = (m1[1][1] * m1[2][2] - m1[2][1] * m1[1][2]);
  mat[1][0] = -(m1[1][0] * m1[2][2] - m1[2][0] * m1[1][2]);
  mat[2][0] = (m1[1][0] * m1[2][1] - m1[2][0] * m1[1][1]);

  mat[0][1] = -(m1[0][1] * m1[2][2] - m1[2][1] * m1[0][2]);
  mat[1][1] = (m1[0][0] * m1[2][2] - m1[2][0] * m1[0][2]);
  mat[2][1] = -(m1[0][0] * m1[2][1] - m1[2][0] * m1[0][1]);

  mat[0][2] = (m1[0][1] * m1[1][2] - m1[1][1] * m1[0][2]);
  mat[1][2] = -(m1[0][0] * m1[1][2] - m1[1][0] * m1[0][2]);
  mat[2][2] = (m1[0][0] * m1[1][1] - m1[1][0] * m1[0][1]);

  return mat;
}

template<typename T, unsigned int M>
Matrix<T, M, M> matrixAdjoint(const Matrix<T, M, M>& m1) {
  Matrix<T, M, M> mat;
  
  for (unsigned int r = 0; r < M; ++r) {
    for (unsigned int c = 0; c < M; ++c) {
      if ((r + c) % 2 == 0) {
        rm[c][r] = matrix_impl::computeDetDetExclude(m1, r, c);
      } else {
        rm[c][r] = -matrix_impl::computeDetDetExclude(m1, r, c);
      }
    }
  }

  return mat;
}

template<typename T, unsigned int M>
Matrix<T, M, M> matrixInverse(const Matrix<T, M, M>& m1) {
  const auto det = matrixDet(m1);
  if (matrix_impl::equalZero(det)) {
    throw MatrixInverseException("matrixInverse det is zero");
  }

  const auto detInverse = T(1.0) / det;

  Matrix<T, M, M> adjoint = matrixAdjoint(m1);
  adjoint *= detInverse;
  return adjoint;
}

template<typename T, unsigned int Rows, unsigned int Cols>
inline T Matrix<T, Rows, Cols>::det() const {
  static_assert(Rows == Cols, "Matrix::det Rows != Cols");
  return matrixDet<T, Rows>(*this);
}

template<typename T, unsigned int Rows, unsigned int Cols>
inline Matrix<T, Rows, Cols> Matrix<T, Rows, Cols>::inverse() const {
  static_assert(Rows == Cols, "Matrix::inverse Rows != Cols");
  return matrixInverse<T, Rows>(*this);
}

template<typename T, unsigned int Rows, unsigned int Cols>
bool Matrix<T, Rows, Cols>::isIdentify() const {
  static_assert(Rows == Cols, "Matrix::isIdentify Rows != Cols");
  const unsigned M = Rows;
  for (unsigned int r = 0; r < M; ++r) {
    for (unsigned int c = 0; c < M; ++c) {

      const auto val = m_[r][c];
      if (r == c) {
        if (!matrix_impl::equalZero(val - T(1))) {
          return false;
        }
      } else if (!matrix_impl::equalZero(val)) {
        return false;
      }
    }
  }
  return true;
}

template<typename T, unsigned int Rows, unsigned int Cols>
inline Matrix<T, Rows, Cols> Matrix<T, Rows, Cols>::transpose() const {
  static_assert(Rows == Cols, "Matrix::transpose Rows != Cols");
  return matrixTranspose<T, Rows, Cols>(*this);
}

template<typename T, unsigned int Rows, unsigned int Cols>
inline Matrix<T, Rows, Cols> Matrix<T, Rows, Cols>::adjoint() const {
  static_assert(Rows == Cols, "Matrix::adjoint Rows != Cols");
  return matrixAdjoint<T, Rows, Cols>(*this);
}

//throws MatrixInverseException
//sove the CX = Y
template<typename T, unsigned int M, unsigned int Cols>
inline Matrix<T, M, Cols> matrixSolve(const Matrix<T, M, M>& c, const Matrix<T, M, Cols>& y)  {
  return c.inverse() * y;// return Matrix for X
}


template<typename T, unsigned int M>
Matrix<T, M, M> buildIdentifyMatrix();


template<typename T>
Matrix<T, 2U, 2U> buildIdentifyMatrix2x2() {
  return Matrix4x4FD {1,0,
                      0,1};
}

template<typename T>
Matrix<T, 3U, 3U> buildIdentifyMatrix3x3() {
  return Matrix4x4FD {1,0,0,
                      0,1,0,
                      0,0,1};
}

template<typename T>
Matrix<T, 4U, 4U> buildIdentifyMatrix4x4() {
  return Matrix4x4FD {1,0,0,0,
                      0,1,0,0,
                      0,0,1,0,
                      0,0,0,1};
}


//angle: degree!
template<typename T>
Matrix<T, 4U, 4U> buildRotateMatrix4x4YXZ(double angley, double anglex, double anglez) {
  assert(std::is_floating_point<T>::value);

  const auto cosx = ::cos(anglex);
  const auto sinx = ::sin(anglex);

  const auto cosy = ::cos(angley);
  const auto siny = ::sin(angley);

  const auto cosz = ::cos(anglez);
  const auto sinz = ::sin(anglez);

  //rotate with: YXZ
  Matrix<T, 4U, 4U> rotateYMat{cosy,  0, -siny,  0,
                               0,     1,  0,    0,
                               siny,  0,  cosy, 0,
                               0,     0,   0,   1};

  Matrix<T, 4U, 4U> rotateXMat{1, 0,     0,    0,
                               0, cosx,  sinx, 0,
                               0, -sinx, cosx, 0,
                               0, 0,     0,    1};


  Matrix<T, 4U, 4U> rotateZMat{cosz, sinz, 0,  0,
                               -sinz, cosz, 0, 0,
                               0,     0,    1, 0,
                               0,     0,    0, 1};

  return rotateYMat * rotateXMat * rotateZMat;
}

template<typename T>
Matrix<T, 4U, 4U> buildTranslateMatrix4x4(T x, T y, T z) {
  Matrix<T, 4U, 4U> translateMat = {1, 0, 0, 0,
                                    0, 1, 0, 0,
                                    0, 0, 1, 0,
                                    x, y, z, 1};

    return translateMat;
}

template<typename T>
Matrix<T, 4U, 4U> buildScaleMatrix4x4(T sx, T sy, T sz) {
  Matrix<T, 4U, 4U> scaleMat = {sx, 0,  0,  0,
                                    0,  sy, 0,  0,
                                    0,  0,  sz, 0,
                                    0,  0,  0,  1};

  return scaleMat;
}

typedef Matrix<float, 1U, 2U> Matrix1x2F;
typedef Matrix<float, 1U, 3U> Matrix1x3F;
typedef Matrix<float, 1U, 4U> Matrix1x4F;
typedef Matrix<float, 2U, 2U> Matrix2x2F;
typedef Matrix<float, 3U, 3U> Matrix3x3F;
typedef Matrix<float, 4U, 4U> Matrix4x4F;

typedef Matrix<double, 1U, 2U> Matrix1x2FD;
typedef Matrix<double, 1U, 3U> Matrix1x3FD;
typedef Matrix<double, 1U, 4U> Matrix1x4FD;
typedef Matrix<double, 2U, 2U> Matrix2x2FD;
typedef Matrix<double, 3U, 3U> Matrix3x3FD;
typedef Matrix<double, 4U, 4U> Matrix4x4FD;

#undef CHECK_THROW

}// namespace s3d

#endif // !S3D_MATH_MATRIX_H
