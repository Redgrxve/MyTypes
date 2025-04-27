#ifndef MATRIX2D_H
#define MATRIX2D_H

#include <iosfwd>

class Matrix2D
{
public:
	Matrix2D(size_t rows, size_t cols);
	Matrix2D(const Matrix2D& other);
	~Matrix2D();

	inline size_t rows() const							{ return m_rows; }
	inline size_t cols() const							{ return m_cols; }
	inline size_t totalSize() const						{ return m_rows * m_cols; }
	inline double at(size_t row, size_t col) const		{ return m_matrix[row][col]; }
	inline void set(double num, size_t row, size_t col) { m_matrix[row][col] = num; }

	void print(std::ostream& os);
	
	Matrix2D transpose() const;
	Matrix2D add(const Matrix2D& other) const;
	Matrix2D sub(const Matrix2D& other) const;
	Matrix2D multiplyBy(const Matrix2D& other) const;
	Matrix2D multiplyBy(double num) const;
	
	static double** createMatrix(size_t rows, size_t cols);
	static void deleteMatrix(double** matrix, size_t rows);

private:
	double** m_matrix;
	size_t m_rows;
	size_t m_cols;
};

#endif
