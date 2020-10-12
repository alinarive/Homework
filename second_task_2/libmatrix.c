#include <stdio.h>
#include <stdlib.h>

int read_matrix(int rows, int columns, int matrix[rows][columns]){
	printf("Enter %d rows of %d elements, each row starting new string\n", rows, columns);
	int i, j, number=0;
	char symbol;
	for (i=0;i<rows;i++){
		for (j=0;j<columns;j++){
			number = scanf("%d", &(matrix[i][j]));
			if (!number) return 0;
		}
	}
	return number;
}

void print_matrix(int rows, int columns, int matrix[rows][columns]){
	/*printf("The result matrix:\n");*/
	int i, j;
	for (i=0;i<rows;i++){
		for (j=0;j<columns;j++){
			printf("%d ", matrix[i][j]);
		}
		printf("\n");
	}
	
}

void add_matrices(int rows, int columns, int matrix1[rows][columns],
		int matrix2[rows][columns], int result_matrix[rows][columns]){
	int i, j;
	for (i=0;i<rows;i++){
		for (j=0;j<columns;j++){
			result_matrix[i][j]=matrix1[i][j]+matrix2[i][j];
		}
	}
}

void multiply_matrix_number(int rows, int columns, float number,
		int matrix[rows][columns], int result_matrix[rows][columns]){
	int i, j;
	for (i=0;i<rows;i++){
		for (j=0;j<columns;j++){
			result_matrix[i][j]=matrix[i][j]*number;
		}
	}
}

void multiply_matrices(int rows, int columns, int columns_second, 
	int matrix1[rows][columns], int matrix2[columns][columns_second],
	int result_matrix[rows][columns_second]){
	int i, j, k;
	for (i=0;i<rows;i++){
		for (j=0;j<columns_second;j++){
			result_matrix[i][j]=0;
			for (k=0;k<columns;k++)
				result_matrix[i][j]+=matrix1[i][k]*matrix2[k][j];
		}
	}
}

void get_minor(int rows, int row, int column, int matrix[rows][rows],
			int new_matrix[rows-1][rows-1]){
	int shift_row=0, shift_column=0, i, j;
	for (i=0;i<rows-1;i++){
		if (i==row)
			shift_row=1;
		shift_column=0;
		for (j=0;j<rows-1;j++){
			if (j==column)
				shift_column=1;
			new_matrix[i][j]=matrix[i+shift_row][j+shift_column];
		}
	}
}

int determinant(int rows, int matrix[rows][rows]){
	int count_determinant=0, degree=1, i;
	if (rows==1) return matrix[0][0];
	if (rows==2) return matrix[0][0]*matrix[1][1] -
			matrix[0][1]*matrix[1][0];
	int new_matrix[rows-1][rows-1];
	for (i=0;i<rows;i++){
		get_minor(rows, 0, i, matrix, new_matrix);
		count_determinant=counnt_determinant+(degree*matrix[0][i]*determinant(rows-1, new_matrix));
		degree=-degree;
	} 
	return det;
}
