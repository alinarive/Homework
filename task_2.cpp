#include <iostream>
#include <malloc.h>


class Matrix {
    int** matrix_init;
    int rows;
    int columns;
    
    void Initialization(int r, int c)
    {
        this -> matrix_init = new int*[r];
        for (int i = 0; i < r; i++)
            matrix_init[i] = new int[c];
        return;
    }
    
    void DeleteMatrix()
    {
        for (int i = 0; i < rows; i++)
            delete [] matrix_init[i];
        delete [] matrix_init;
        return;
    }
    
public:
    Matrix(int r, int c)
    {
        rows = r;
        columns = c;
        this -> Initialization(rows, columns);
    };
    
    Matrix(int r, int c, int** matrix)
    {
        rows = r;
        columns = c;
        for ( int i = 0; i < r; i++){
            for (int j = 0; j < c; j++)
                matrix_init[i][j] = matrix[i][j];
        }
    }
    
    Matrix(const Matrix& matrix)
    {
        rows = matrix.rows;
        columns = matrix.columns;
        this -> Initialization(rows, columns);
        for (int i = 0; i < matrix.rows; i++){
            for (int j = 0; j < matrix.columns; j++)
                matrix_init[i][j] = matrix.matrix_init[i][j];
        }
    };
    
    Matrix& operator = (const Matrix& matrix)
    {
        this -> DeleteMatrix();
        rows = matrix.rows;
        columns = matrix.columns;
        this -> Initialization(matrix.rows, matrix.columns);
        for (int i = 0; i < rows; i++){
            for (int j = 0; j < columns; j++)
                matrix_init[i][j] = matrix.matrix_init[i][j];
        }
        return *this;
    };
    
    Matrix operator + (const Matrix& matrix_first)
    {
        if ((rows != matrix_first.rows) && (columns != matrix_first.columns)) return matrix_first;
        Matrix matrix_second(rows, columns);
        for (int i = 0; i < matrix_first.rows; i++){
            for (int j = 0; j < columns; j++)
                matrix_second.matrix_init[i][j] = matrix_init[i][j] + matrix_first.matrix_init[i][j];
        }
        return matrix_second;
    }
    
    Matrix operator * (const Matrix& matrix_first)
    {
        if (rows != matrix_first.columns) return matrix_first;
        Matrix matrix_second(matrix_first.rows, columns);
        for (int i = 0; i < matrix_first.rows; i++){
            for (int j = 0; j < columns; j++){
                matrix_second.matrix_init[i][j] = 0;
                for (int k = 0; k< matrix_first.columns; k++)
                    matrix_second.matrix_init += matrix_first.matrix_init[i][k]*matrix_init[k][j];
            }
        }
        return matrix_second;
    }
    
    int* operator [] (int i)
    {
        return matrix_init[i];
    }
    
    friend std::ostream& operator << (std::ostream &out, const Matrix& matrix)
    {
        for (int i = 0; i < matrix.rows; i++){
            for (int j = 0; j < matrix.columns; j++)
                std::cout << matrix.matrix_init[i][j];
            std::cout << std::endl;
        }
        return out;
    };
    
    friend std::istream& operator >> (std::istream &in, const Matrix& matrix)
    {
        for (int i = 0; i < matrix.rows; i++)
            for (int j = 0; j < matrix.columns; j++)
                std::cin >> matrix.matrix_init[i][j];
        return in;
    };

    
    ~Matrix()
    {
        this -> DeleteMatrix();
    }
};

int main()
{
    Matrix matrix_first(1, 1);
    Matrix matrix_second(1, 1);
    Matrix matrix_third(1, 1);
    
    std::cin >> matrix_first;
    std::cin >> matrix_second;
    std::cin >> matrix_third;
    matrix_first = matrix_second + matrix_third;
    matrix_second = matrix_first * matrix_third;
    std::cout << matrix_first;
    std::cout << matrix_second;
    std::cout << matrix_third;
}
    
 
