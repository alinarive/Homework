#include <stdio.h>
#include <stdlib.h>
#include "libmatrix.h"

int main(){
	const char error_input[]="Wrong input, only digits and spaces allowed\n";
	const char invitation_rows[]="Enter number of rows\n";
	const char invitation_columns[]="Enter number of columns\n";
	const char error_rows[]="Wrong number of rows, it should be more then 0, restart program\n";
	const char error_columns[]="Wrong number of columns, it should be more then 0, restart program\n";
	const char error_input_rows[]="Number of rows must be an int\n";
	const char error_input_columns[]="Number of columns must be an int\n";


	int rows=0, columns=0, flag_error=0;
	printf("%s", invitation_rows);
	flag_error=scanf("%d", &rows);
	if (!flag_error){
		printf("%s", error_input_rows);
		return 0;
	}
	if (rows<=0){
		printf("%s", error_rows);
		return 0;
	}
	printf("%s", invitation_columns);
	flag_error=scanf("%d", &columns);
	if (!flag_error){
		printf("%s", error_input_columns);
		return 0;
	}
	if (columns<=0){
		printf("%s", error_columns);
		return 0;
	}	
	int matrix[rows][columns];

	flag_error=read_matrix(rows, columns, matrix);
	if (!flag_error){
		printf("%s", error_input);
		return 0;
	}

	int choice=0;
	printf("If you want multiply matrix to a number enter 1,\nif you want to sum two matrices enter 2,\nif you want to multiply two matrices enter 3,\nif you want to count the determinant of a matrix enter 4, if you want to exit press any key\n");
	flag_error=scanf("%d", &choice);
	switch (choice){
	case 1:{
		float number=0;
		int result_matrix[rows][columns];
		printf("Enter a number to multiply matrix\n");
		scanf("%f", &number);
		multiply_matrix_number(rows, columns, number,
					matrix, result_matrix);
		print_matrix(rows, columns, result_matrix);
	break;
	}
	case 2:{
		int second_matrix_add[rows][columns];
		int result_matrix_add[rows][columns];
		printf("Enter second matrix\n");
		flag_error=read_matrix(rows, columns, second_matrix_add);
		if (!flag_error){
			printf("%s", error_input);
			return 0;
		}
		add_matrices(rows, columns, 
				matrix, second_matrix_add, result_matrix_add);
		print_matrix(rows, columns, result_matrix_add);
	break;
	}
	case 3:{
		int rows_second=0, rows_result=0, columns_second=0, 
			columns_result=0, flag_error=0;
		printf("%s", invitation_rows);
		flag_error=scanf("%d", &rows_second);
		if (!flag_error){
			printf("%s", error_input_rows);
			return 0;
		}
		if (rows_second<=0){
			printf("%s", error_rows);
			return 0;
		}
		if (rows_second!=columns){
			printf("Matricies are incopatible, try again\n");
			return 0;
		}
		printf("%s", invitation_columns);
		flag_error=scanf("%d", &columns_second);
		if (!flag_error){
			printf("%s", error_input_columns);
			return 0;
		}
		if (columns_second<=0){
			printf("%s", error_columns);
			return 0;
		}			
		int second_matrix_multiply[rows_second][columns_second];
		int result_matrix_multiply[rows][columns_second];
		printf("Enter second matrix\n");
		flag_error=read_matrix(rows_second, columns_second, second_matrix_multiply);
		if (!flag_error){
			printf("%s", error_input);
			return 0;
		}
		multiply_matrices(rows, columns, columns_second, matrix, 
				second_matrix_multiply, result_matrix_multiply);
		print_matrix(rows, columns_second, result_matrix_multiply);
	break;
	}

	case 4:{
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
	break;
	}
	default:
		printf("Bye!\n");
		return 0;
	}
}
