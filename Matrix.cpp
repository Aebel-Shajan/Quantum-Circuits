#include<vector>
#include<iostream>
#include<complex>
#include "Matrix.h"


// Helper functions
void throw_dimension_error(const Matrix& m1, const Matrix& m2, const std::string& operation) {
    std::string error_message{"Matrix dimensions do not match for " + operation + ": (" \
        + std::to_string(m1.get_rows()) + "," + std::to_string(m1.get_cols()) + ") (" \
        + std::to_string(m2.get_rows()) + "," + std::to_string(m2.get_cols()) + ")"};
    throw std::invalid_argument(error_message);
}


// Matrix Class
Matrix& Matrix::operator=(const Matrix& m) {
    if (this != &m) {
        rows = m.rows;
        cols = m.cols;
        data = m.data;
    }
    return *this;
}

/**
 * @brief Adds two matrices together element-wise.
 * 
 * @param m 
 * @return Matrix 
 * @throw std::invalid_argument if the dimensions of the matrices do not match.
 */
Matrix Matrix::operator+(const Matrix& m) {
    if (rows != m.rows || cols != m.cols){
        throw_dimension_error(*this, m, "addition");
    }
    Matrix result(rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result.data[i][j] = data[i][j] + m.data[i][j];
        }
    }
    return result;
}

/**
 * @brief Subtracts two matrices element-wise.
 * 
 * @param m 
 * @return Matrix 
 * @throw std::invalid_argument if the dimensions of the matrices do not match.
 */
Matrix Matrix::operator-(const Matrix& m) {
    if (rows != m.rows || cols != m.cols){
        throw_dimension_error(*this, m, "subtraction");
    }
    Matrix result(rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result.data[i][j] = data[i][j] - m.data[i][j];
        }
    }
    return result;
}

/**
 * @brief Multiplies two matrices together.
 * 
 * @param m 
 * @return Matrix 
 * @throw std::invalid_argument if the columns of the first matrix does not match
 * the rows of the second matrix.
 */
Matrix Matrix::operator*(const Matrix& m) {
    if (cols != m.rows) {
        throw_dimension_error(*this, m, "multiplication");
    }
    Matrix result(rows, m.cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < m.cols; j++) {
            for (int k = 0; k < cols; k++) {
                result.data[i][j] += data[i][k] * m.data[k][j];
            }
        }
    }
    return result;
}

/**
 * @brief Returns the element at the specified row and column of the matrix.
 * 
 * @param r 
 * @param c 
 * @return const std::complex<double> 
 * @throw std::out_of_range if the row or column is out of bounds.
 */
const std::complex<double> Matrix::operator()(size_t r, size_t c) const {
    if (r>=rows || c>=cols) {
        throw std::out_of_range("Index ("+ std::to_string(r) + "," + std::to_string(c) + ") out of bounds");
    }
    return data[r][c];
}

/**
 * @brief Sets the element at the specified row and column of the matrix.
 * 
 * @param r 
 * @param c 
 * @return std::complex<double>& 
 * @throw std::out_of_range if the row or column is out of bounds.
 */
std::complex<double>& Matrix::operator()(size_t r, size_t c) {
    if (r>=rows || c>=cols) {
        throw std::out_of_range("Index ("+ std::to_string(r) + "," + std::to_string(c) + ") out of bounds");
    }
    return data[r][c];
}

/**
 * @brief Overloads the << operator to print the matrix in a readable format.
 * 
 * @param os 
 * @param matrix 
 * @return std::ostream& 
 */
std::ostream & operator<<(std::ostream &os, const Matrix& matrix) {
    std::vector<std::vector<std::string>> string_matrix(matrix.rows, std::vector<std::string>(matrix.cols));
    int max_length{0};
    for(int i{}; i<matrix.rows; i++) {
        for(int j{}; j<matrix.cols; j++) {
            double real = matrix.data[i][j].real();
            double imag = matrix.data[i][j].imag();
            char sign = imag >= 0 ? '+' : '-';
            imag = imag >= 0 ? imag : -imag;
            real = real == 0 ? 0 : real; // fix for -0 (which is a thing, like y)
            if (imag == 0) {
                string_matrix[i][j] = std::to_string(real);
            } else {
                string_matrix[i][j] = std::to_string(real) + sign + std::to_string(imag) + "i";
            }
            if (string_matrix[i][j].length() > max_length) {
                max_length = string_matrix[i][j].length();
            }
        }
    }
    if(matrix.rows > 0 & matrix.cols > 0) {
        os<<"["<<std::endl;
        for(int i{}; i<matrix.rows; i++) {
            os<<"  ";
            for(int j{}; j<matrix.cols; j++) {
                os<<string_matrix[i][j];
                for(int k{}; k<max_length-string_matrix[i][j].length()+1; k++) {
                    os<<" ";
                }
                os << ", ";
            }
            os<<std::endl;
        }
        os<<" ]"<<std::endl;
    } else {
        os<<"[]"<<std::endl;
    }
    return os;
}

/**
 * @brief Reads a matrix from an input consisting of complex numbers or doubles seperated 
 * by spaces. Complex numbers are represent by (real,imaginary) where real and imaginary 
 * are doubles.
 * 
 * 
 * @param is 
 * @param matrix 
 * @return std::istream& 
 * @throw std::invalid_argument if the input is not valid.
 */
std::istream& operator>>(std::istream& is, Matrix& matrix){
    for(int i{}; i<matrix.rows; i++) {
        for(int j{}; j<matrix.cols; j++)
        {
            try {
                is>>matrix.data[i][j];
            } catch (std::invalid_argument& e) {
                throw std::invalid_argument("Invalid input for matrix at (" + std::to_string(i) + "," + std::to_string(j) + ")");
            }
        }
    }
    return is;
}

/**
 * @brief Returns the tensor product of two matrices. The tensor product of two matrices
 * A and B is a matrix of size (rows(A)*rows(B), cols(A)*cols(B)) where the matrix at
 * (i,j) is the product of the element at (i,j) of A and the matrix B.
 * 
 * @param m 
 * @return Matrix 
 */
Matrix Matrix::tensor_product(const Matrix& m) {
    Matrix result(rows*m.rows, cols*m.cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            for (int u = 0; u < m.rows; u++) {
                for (int v = 0; v < m.cols; v++) {
                    result.data[u*rows + i][v*cols+j] = data[i][j] * m.data[u][v];
                }
            }
        }
    }
    return result;
}


// Matrix Functions
/**
 * @brief Returns a square identity matrix of size n.
 * 
 * @param n 
 * @return Matrix 
 */
Matrix identity_matrix(size_t n) {
    Matrix result(n, n);
    for (size_t i = 0; i < n; i++) {
        result(i, i) = 1;
    }
    return result;
}

