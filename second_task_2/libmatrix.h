/*function void to read a matrix from a keyboard, needs amount of rows -> rows,
	amount of columns -> columns, array where to put values -> matrix*/
int read_matrix(int rows, int columns, int matrix[rows][columns]);

/*function void to print a matrix, needs amount of rows -> rows,
	amount of columns -> columns, array from where to read values -> matrix*/
void print_matrix(int rows, int columns, int matrix[rows][columns]);

/*function void to summ two matrices and put the result into third,
	needs amount of rows -> rows, amount of columns -> columns,
	first matrix to summ -> matrix1, second matrix to summ -> matrix2, 
	a matrix to put result into -> result_matrix*/
void add_matrices(int rows, int columns, int matrix1[rows][columns],
		int matrix2[rows][columns], int result_matrix[rows][columns]);

/*function void to multiply a matrix to a number, needs amount of rows -> rows,
	amount of columns -> columns, a number to multiply -> number,
	a matrix to multiply -> matrix, a matrix to put result into -> result_matrix*/
void multiply_matrix_number(int rows, int columns, float number,
		int matrix[rows][columns], int result_matrix[rows][columns]);

/*function void to multiply two matrices, needs amount of rows -> rows, 
	amount of columns in the first matrix -> columns, amount of columns in 
	the second matrix -> columns_second, first matrix to multiply -> matrix1,
	second matrix to multiply -> matrix2, matrix to put result into -> result_matrix*/
void multiply_matrices(int rows, int columns, int columns_second, 
	int matrix1[rows][columns], int matrix2[columns][columns_second],
	int result_matrix[rows][columns_second]);

/*function void to get a complementary minor to a column and a row from a matrix,
	needs amount of rows -> rows, number of a row -> row, number of a 
	column -> column, matrix from which to extrac the minor -> matrix,
	matrix where to extrac a minor -> new_matrix*/ 
void get_minor(int rows, int row, int column, int matrix[rows][rows],
			int new_matrix[rows-1][rows-1]);

/*function which returns determinant of a matrix (int), needs amount of rows ->
	rows, matrix to count its determinant -> matrix*/
int determinant(int rows, int matrix[rows][rows]);
