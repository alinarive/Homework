int read_matrix(int rows, int columns, int matrix[rows][columns]);

void print_matrix(int rows, int columns, int matrix[rows][columns]);

void add_matrices(int rows, int columns, int matrix1[rows][columns],
		int matrix2[rows][columns], int result_matrix[rows][columns]);

void multiply_matrix_number(int rows, int columns, float number,
		int matrix[rows][columns], int result_matrix[rows][columns]);

void multiply_matrices(int rows, int columns, int columns_second, 
	int matrix1[rows][columns], int matrix2[columns][columns_second],
	int result_matrix[rows][columns_second]);

void get_minor(int rows, int row, int column, int matrix[rows][rows],
			int new_matrix[rows-1][rows-1]);

int determinant(int rows, int matrix[rows][rows]);
