#ifndef Matrix_H
#define Matrix_H
#include <iostream>
#include <vector>
#include <complex>

/**
 * @brief A class for representing a matrix of complex numbers.
 *
 */
class Matrix
{
    // friend functions
    friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix);
    friend std::istream& operator>>(std::istream& is, Matrix& matrix);
private:
    size_t rows;
    size_t cols;
    std::vector<std::vector<std::complex<double>>> data;

public:
    // Constructors and destructors
    Matrix();
    Matrix(size_t rows, size_t cols);
    Matrix(std::vector<std::vector<std::complex<double>>> data);
    ~Matrix() {}

    // Accessors
    size_t get_rows() const { return rows; }
    size_t get_cols() const { return cols; }
    const std::complex<double> operator()(size_t, size_t) const;

    // Mutators
    Matrix& operator=(const Matrix&);
    Matrix operator+(const Matrix&);
    Matrix operator-(const Matrix&);
    Matrix operator*(const Matrix&);
    bool operator==(const Matrix);
    std::complex<double>& operator()(size_t, size_t);
    Matrix tensor_product(const Matrix&);
    Matrix transpose();
    Matrix conjugate();
    Matrix adjoint();
};

// Non-member functions
Matrix identity_matrix(size_t n);
Matrix perform_tensor_product(const std::vector<Matrix> matrices);
#endif
