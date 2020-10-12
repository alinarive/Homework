#include <stdio.h>
#include <stdlib.h>
#include "libmatrix.h"

int main(){
	int rows=0, columns=0, flag_error=0;
	printf("Enter number of rows\n");
	flag_error=scanf("%d", &rows);
	if (!flag_error){
		printf("Number of rows must be an int\n");
		return 0;
	}
	if (rows<=0){
		printf("Wrong number of rows, it should be more then 0, restart program\n");
		return 0;
	}
	printf("Enter number of columns\n");
	flag_error=scanf("%d", &columns);
	if (!flag_error){
		printf("Number of columns must be an int\n");
		return 0;
	}
	if (columns<=0){
		printf("Wrong number of columns, it should be more then 0, restart program\n");
		return 0;
	}	
	int matrix[rows][columns];

	flag_error=read_matrix(rows, columns, matrix);
	if (!flag_error){
		printf("Wrong input, only digits and spaces allowed\n");
		return 0;
	}

	int choice=0;
	printf("If you want multiply matrix to a number enter 1,\n if you want to sum two matrices enter 2,\nif you want to multiply two matrices enter 3,\nif you want to count the determinant of a matrix enter 4, if you want to exit press any key\n");
	flag_error=scanf("%d", &choice);
	if (!flag_error){
		printf("Bye!\n");
		return 0;
	}
	
	if  (choice==1){
		float number=0.0;
		int result_matrix[rows][columns];
		printf("Enter a number to multiply matrix\n");
		scanf("%f", &number);
		multiply_matrix_number(rows, columns, number,
					matrix, result_matrix);
		print_matrix(rows, columns, result_matrix);
	}

	if (choice==2){
		int second_matrix_add[rows][columns];
		int result_matrix_add[rows][columns];
		printf("Enter second matrix\n");
		flag_error=read_matrix(rows, columns, second_matrix_add);
		if (!flag_error){
			printf("Wrong input, only digits and spaces allowed\n");
			return 0;
		}
		add_matrices(rows, columns, 
				matrix, second_matrix_add, result_matrix_add);
		print_matrix(rows, columns, result_matrix_add);
	}

	if (choice==3){
		int rows_second=0, rows_result=0, columns_second=0, 
			columns_result=0, flag_error=0;
		printf("Enter number of rows\n");
		scanf("%d", &rows_second);
		if (rows_second<=0){
			printf("Wrong number of rows, it should be more then 0, restart program\n");
			return 0;
		}
		if (rows_second!=columns){
			printf("Matricies are incopatible, try again\n");
			return 0;
		}
		printf("Enter number of columns\n");
		scanf("%d", &columns_second);
		if (columns_second<=0){
			printf("Wrong number of columns, it should be more then 0, restart program\n");
			return 0;
		}			
		int second_matrix_multiply[rows_second][columns_second];
		int result_matrix_multiply[rows][columns_second];
		printf("Enter second matrix\n");
		flag_error=read_matrix(rows_second, columns_second, second_matrix_multiply);
		if (!flag_error){
			printf("Wrong input, only digits and spaces allowed\n");
			return 0;
		}
		multiply_matrices(rows, columns, columns_second, matrix, 
				second_matrix_multiply, result_matrix_multiply);
		print_matrix(rows, columns_second, result_matrix_multiply);
	}

	if (choice==4){
		if (rows!=columns){
			printf("Determinant can be found only for square matricces\n");
			return 0;
		}
		else{
			int det=0;
			det=determinant(rows, matrix);
			printf("%d is the determinant for matrix\n", det);
			print_matrix(rows, columns, matrix);
		}
	}
	printf("Bye!\n");
	return 0;
}
