#include "matrix2d.h"
#include <ostream>

//Matrix2D::Matrix2D()
//	: m_matrix(nullptr), m_rows(0), m_cols(0) {}

Matrix2D::Matrix2D(size_t rows, size_t cols)
	: m_rows(rows), m_cols(cols)
{
	m_matrix = new double*[rows]{};
	for (size_t i = 0; i < rows; ++i)
		m_matrix[i] = new double[cols]{};
}

Matrix2D::Matrix2D(const Matrix2D& other) 
	: m_rows(other.m_rows), m_cols(other.m_cols)
{
	m_matrix = new double* [m_rows]{};
	for (size_t i = 0; i < m_rows; ++i) {
		m_matrix[i] = new double[m_cols];
		for (int j = 0; j < m_cols; ++j)
			m_matrix[i][j] = other.at(i, j);
	}
}

Matrix2D::~Matrix2D()
{
	for (size_t i = 0; i < m_rows; ++i)
		delete[] m_matrix[i];
	delete[] m_matrix;
}

void Matrix2D::print(std::ostream& os)
{
	for (size_t i = 0; i < m_rows; ++i) {
		for (size_t j = 0; j < m_cols; ++j) {
			os << m_matrix[i][j] << "\t";
		}
		os << "\n";
	}
}

Matrix2D Matrix2D::transpose() const
{
	Matrix2D transposed(m_cols, m_rows);
	for (size_t i = 0; i < m_rows; ++i)
		for (size_t j = 0; j < m_cols; j++)
			transposed.set(at(i, j), j, i);

	return transposed;
}

Matrix2D Matrix2D::add(const Matrix2D& other) const
{
	if (m_rows != other.m_rows || m_cols != other.m_cols)
		return *this;

	Matrix2D result(m_rows, m_cols);
	for (size_t i = 0; i < m_rows; ++i)
		for (size_t j = 0; j < m_cols; ++j)
			result.m_matrix[i][j] = m_matrix[i][j] + other.at(i, j);
	return result;
}

Matrix2D Matrix2D::sub(const Matrix2D& other) const
{
	if (m_rows != other.m_rows || m_cols != other.m_cols)
		return *this;

	Matrix2D result(m_rows, m_cols);
	for (size_t i = 0; i < m_rows; ++i)
		for (size_t j = 0; j < m_cols; ++j)
			result.m_matrix[i][j] = m_matrix[i][j] - other.at(i, j);
	return result;
}

Matrix2D Matrix2D::multiplyBy(const Matrix2D& other) const
{
	if (m_cols != other.m_rows)
		return *this;

	Matrix2D result(m_rows, other.m_cols);
	for (size_t i = 0; i < m_rows; ++i) {
		for (size_t j = 0; j < other.m_cols; ++j) {
			for (size_t k = 0; k < m_cols; ++k)
				result.m_matrix[i][j] += m_matrix[i][k] * other.m_matrix[k][j];
		}
	}
	return result;
}

Matrix2D Matrix2D::multiplyBy(double num) const
{
	Matrix2D result(*this);
	for (size_t i = 0; i < m_rows; ++i)
		for (size_t j = 0; j < m_cols; ++j)
			result.m_matrix[i][j] *= num;
	return result;
}

double** Matrix2D::createMatrix(size_t rows, size_t cols)
{
	double** matrix = new double* [rows];
	for (size_t i = 0; i < rows; ++i)
		matrix[i] = new double[cols];

	return matrix;
}

void Matrix2D::deleteMatrix(double** matrix, size_t rows)
{
	for (size_t i = 0; i < rows; ++i)
		delete[] matrix[i];
	delete[] matrix;
}

//Matrix2D Matrix2D::sum(const Matrix2D& m1, const Matrix2D& m2)
//{
//	Matrix2D result(m1.rows(), m1.cols());
//	for (size_t i = 0; i < m1.rows(); ++i)
//		for (size_t j = 0; j < m1.cols(); ++j)
//			result.set(m1.at(i, j) + m2.at(i, j), i, j);
//	return result;
//}
//
//Matrix2D Matrix2D::subtract(const Matrix2D& m1, const Matrix2D& m2)
//{
//	Matrix2D result(m1.rows(), m1.cols());
//	for (size_t i = 0; i < m1.rows(); ++i)
//		for (size_t j = 0; j < m1.cols(); ++j)
//			result.set(m1.at(i, j) - m2.at(i, j), i, j);
//	return result;
//}
//
//Matrix2D Matrix2D::product(const Matrix2D& m1, const Matrix2D& m2)
//{
//	Matrix2D result(m1.rows(), m2.cols());
//	for (size_t i = 0; i < m1.rows(); ++i) 
//		for (size_t j = 0; j < m2.cols(); ++j) 
//			for (size_t k = 0; k < m1.cols(); ++k) 
//				result.m_matrix[i][j] += m1.m_matrix[i][k] * m2.m_matrix[k][j];
//
//	return result;
//}
//

