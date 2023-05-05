#ifndef Matrix_H
#define Matrix_H
#include <iostream>
#include <vector>
#include <complex>

/**
 * @brief A class representing a matrix of complex numbers.
 * 
 */
class Matrix {
    // friend functions
    friend std::ostream & operator<<(std::ostream &os, const Matrix& matrix);
    friend std::istream & operator>>(std::istream& is, Matrix& matrix);
    private:
        size_t rows;
        size_t cols;
        std::vector<std::vector<std::complex<double>>> data;
    public:
        // Constructors and destructors
        Matrix() : rows{0}, cols{0} {
            data = std::vector<std::vector<std::complex<double>>>(rows, std::vector<std::complex<double>>(cols));
        }
        Matrix(size_t r, size_t c) : rows{r}, cols{c} {
            data = std::vector<std::vector<std::complex<double>>>(rows, std::vector<std::complex<double>>(cols));
        }
        Matrix(std::vector<std::vector<std::complex<double>>> data_in) : data{data_in} {
            rows = data.size();
            cols = data[0].size();
        }
        ~Matrix() {}

        // Accessors
        size_t get_rows() const {return rows;}
        size_t get_cols() const {return cols;}
        const std::complex<double> operator()(size_t, size_t) const;

        // Mutators
        Matrix& operator=(const Matrix&);
        Matrix operator+(const Matrix&);
        Matrix operator-(const Matrix&);
        Matrix operator*(const Matrix&);
        std::complex<double>& operator()(size_t, size_t);
        Matrix tensor_product(const Matrix&);
};

// non-member functions
Matrix identity_matrix(size_t n);
#endif
