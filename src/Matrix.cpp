#include "Matrix.h"

///////////////////////////////////////////////////////////////////////////////
// Helper functions
///////////////////////////////////////////////////////////////////////////////

void throw_dimension_error(const Matrix& m1, const Matrix& m2, const std::string& operation)
{
    std::string error_message{"Matrix dimensions do not match for "+operation+": ("+std::to_string(m1.get_rows())+","+std::to_string(m1.get_cols())+") ("+std::to_string(m2.get_rows())+","+std::to_string(m2.get_cols())+")"};
    throw std::invalid_argument(error_message);
}


///////////////////////////////////////////////////////////////////////////////
// Matrix Class
///////////////////////////////////////////////////////////////////////////////

Matrix::Matrix() : Matrix(0, 0) {}

Matrix::Matrix(size_t r, size_t c) : rows{ r }, cols{ c }
{
    data=std::vector<std::vector<std::complex<double>>>(rows, std::vector<std::complex<double>>(cols, std::complex<double>(0.0, 0.0)));
}

Matrix::Matrix(std::vector<std::vector<std::complex<double>>> data_in) : data{ data_in }
{
    rows=data.size();
    cols=data[0].size();
}

Matrix& Matrix::operator=(const Matrix& m)
{
    if (this!=&m)
    {
        rows=m.rows;
        cols=m.cols;
        data=m.data;
    }
    return *this;
}

Matrix Matrix::operator+(const Matrix& m)
{
    if (rows!=m.rows||cols!=m.cols)
    {
        throw_dimension_error(*this, m, "addition");
    }
    Matrix result(rows, cols);
    for (int i=0; i<rows; i++)
    {
        for (int j=0; j<cols; j++)
        {
            result(i, j)=data[i][j]+m(i, j);
        }
    }
    return result;
}

Matrix Matrix::operator-(const Matrix& m)
{
    if (rows!=m.rows||cols!=m.cols)
    {
        throw_dimension_error(*this, m, "subtraction");
    }
    Matrix result(rows, cols);
    for (int i=0; i<rows; i++)
    {
        for (int j=0; j<cols; j++)
        {
            result(i, j)=data[i][j]-m(i, j);
        }
    }
    return result;
}

Matrix Matrix::operator*(const Matrix& m)
{
    if (cols!=m.rows)
    {
        throw_dimension_error(*this, m, "multiplication");
    }
    Matrix result(rows, m.cols);
    for (int i=0; i<rows; i++)
    {
        for (int j=0; j<m.cols; j++)
        {
            for (int k=0; k<cols; k++)
            {
                result(i, j)+=data[i][k]*m(k, j);
            }
        }
    }
    return result;
}

const std::complex<double> Matrix::operator()(size_t r, size_t c) const
{
    if (r>=rows||c>=cols)
    {
        throw std::out_of_range("Index ("+std::to_string(r)+","+std::to_string(c)+") out of bounds");
    }
    return data[r][c];
}

std::complex<double>& Matrix::operator()(size_t r, size_t c)
{
    if (r>=rows||c>=cols)
    {
        throw std::out_of_range("Index ("+std::to_string(r)+","+std::to_string(c)+") out of bounds");
    }
    return data[r][c];
}

std::ostream& operator<<(std::ostream& os, const Matrix& matrix)
{
    std::vector<std::vector<std::string>> string_matrix(matrix.rows, std::vector<std::string>(matrix.cols));
    int max_length{ 0 };
    for (int i{}; i<matrix.rows; i++)
    {
        for (int j{}; j<matrix.cols; j++)
        {
            double real=matrix.data[i][j].real();
            double imag=matrix.data[i][j].imag();
            char sign=imag>=0 ? '+' : '-';
            imag=imag>=0 ? imag : -imag;
            real=real==0 ? 0 : real; // fix for -0 (which is a thing, like y)
            if (imag==0)
            {
                string_matrix[i][j]=std::to_string(real);
            }
            else
            {
                string_matrix[i][j]=std::to_string(real)+sign+std::to_string(imag)+"i";
            }
            if (string_matrix[i][j].length()>max_length)
            {
                max_length=string_matrix[i][j].length();
            }
        }
    }
    if (matrix.rows>0&matrix.cols>0)
    {
        os<<"["<<std::endl;
        for (int i{}; i<matrix.rows; i++)
        {
            os<<"  ";
            for (int j{}; j<matrix.cols; j++)
            {
                os<<string_matrix[i][j];
                for (int k{}; k<max_length-string_matrix[i][j].length()+1; k++)
                {
                    os<<" ";
                }
                os<<", ";
            }
            os<<std::endl;
        }
        os<<" ]"<<std::endl;
    }
    else
    {
        os<<"[]"<<std::endl;
    }
    return os;
}

std::istream& operator>>(std::istream& is, Matrix& matrix)
{
    for (int i{}; i<matrix.rows; i++)
    {
        for (int j{}; j<matrix.cols; j++)
        {
            try
            {
                is>>matrix.data[i][j];
            }
            catch (std::invalid_argument& e)
            {
                throw std::invalid_argument("Invalid input for matrix at ("+std::to_string(i)+","+std::to_string(j)+")");
            }
        }
    }
    return is;
}

bool Matrix::operator==(const Matrix m) {
    if (rows!=m.get_rows()||cols!=m.get_cols())
    {
        return false;
    }
    for (int i{}; i<rows; i++)
    {
        for (int j{}; j<cols; j++)
        {
            double tol=1e-10;
            if (abs(data[i][j].real()-m(i, j).real())>tol||abs(data[i][j].imag()-m(i, j).imag())>tol)
            {
                return false;
            }
        }
    }
    return true;
}

Matrix Matrix::tensor_product(const Matrix& m)
{
    Matrix result(rows*m.rows, cols*m.cols);
    for (int i=0; i<rows; i++)
    {
        for (int j=0; j<cols; j++)
        {
            for (int u=0; u<m.rows; u++)
            {
                for (int v=0; v<m.cols; v++)
                {
                    result(u*rows+i, v*cols+j)=data[i][j]*m(u, v);
                }
            }
        }
    }
    return result;
}

Matrix Matrix::transpose()
{
    Matrix result(cols, rows);
    for (int i=0; i<rows; i++)
    {
        for (int j=0; j<cols; j++)
        {
            result(j, i)=data[i][j];
        }
    }
    return result;
}

Matrix Matrix::conjugate()
{
    Matrix result(rows, cols);
    for (int i=0; i<rows; i++)
    {
        for (int j=0; j<cols; j++)
        {
            result(i, j)=std::conj(data[i][j]);
        }
    }
    return result;
}

Matrix Matrix::adjoint()
{
    return transpose().conjugate();
}


///////////////////////////////////////////////////////////////////////////////
// Non-member functions
///////////////////////////////////////////////////////////////////////////////

Matrix identity_matrix(size_t n)
{
    Matrix result(n, n);
    for (size_t i=0; i<n; i++)
    {
        result(i, i)=std::complex<double>(1, 0);
    }
    return result;
}

Matrix perform_tensor_product(const std::vector<Matrix> matrices)
{
    Matrix result=matrices[0];
    for (size_t i=1; i<matrices.size(); i++)
    {
        result=result.tensor_product(matrices[i]);
    }
    return result;
}