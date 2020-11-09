#include <stdio.h>
#include <stdlib.h>

struct node
{
	char *word;
	struct node *next;
};

void put_out(struct node *list)
{
	while (list){
		struct node *tmp = list;	
        	printf("%s\n", tmp->word);
        	free(tmp->word);
		free(tmp);
        	list = list->next;
    	}
}

void delete(struct node *list)
{
	while (list){
		struct node *temporary = list;
		free(temporary->word);
		free(temporary);
		list = list->next;
	}
}

char* duplicate(char *array, int *length, char symbol) /*duplicate array's length and
						copy*/
{
	char *new_array;
	int i;
	new_array = malloc(*length*2);
	for (i = 0;i < *length;i++)
		*(new_array+i) = *(array + i);
	*(new_array + i) = symbol;
	*length = (*length)*2;
	free(array);
	return new_array;
};

char* optimize(char *array, char *new_array, int length) /*build an array accurate 
						for the word*/
{	int i = 0;
	new_array = malloc(length + 1);
	for(;i < length;i++)
		*(new_array + i) = *(array + i);
	*(new_array + i) = 0;
	return new_array;
}

struct node* new_word(char *array, struct node **first, struct node *last)
{
	struct node *temporary;
	temporary = malloc(sizeof(struct node));
	temporary->word = array;
	temporary->next = NULL;
	if (last){
		last->next = temporary;
		last = last->next;
	}
	else{
		*first = last = temporary;
	}
	return last;
}

void initial_values(int *count, int *size, char **array)
{
	*count = 0;
	*size = 8;
	free(*array);
	*array = malloc(8);
}

void print_error(int *flag, struct node **pointer)
{
	char error[] = "Error, please enter correct number of quotes\n";
	printf("%s", error);
        *flag = 0;
        delete(*pointer);
}

void for_next_itteration(struct node **first, struct node **last)
{
	char invitation[] = "Invitation: ";
	*first = *last = NULL;
	printf("%s", invitation);
}

void met_enter(struct node **first, struct node **last, int *flag)
{  
	if (*flag)
		print_error(flag, first);
        else
        	put_out(*first);
	for_next_itteration(first, last);
}

void met_symbol(int *count, int *size, char **array, int symbol)
{
	if (*count < *size)
        	*(*array + (*count)) = (char)symbol;
        else
                *array = duplicate(*array, size, symbol);
	(*count)++;
}

void met_space(struct node **first, struct node **last, char *array, 
		char **string, int count)
{
	*string = optimize(array, *string, count);
        *last = new_word(*string, first, *last);
}
	 
void separate(FILE *file)
{
	int symbol;
	struct node *first = NULL, *last = NULL;
	int count = 0;				/*counter for word's length*/
	int size = 8;				/*start length*/
	char *array, *str;			/*array of letters*/
	int quotes = 0, ampersand = 0, line = 0, arrow_right = 0, special_symbol = 0,
		double_worked = 0;	
	array = malloc(size);
	for_next_itteration(&first, &last);
	while ((symbol = getc(file)) != EOF){
		if (((symbol != ' ' &&  symbol != '\t' && symbol != '\r' 
			&& symbol != '&' && symbol != '|' && symbol != ';' 
			&& symbol != '>' && symbol != '<' && symbol != '('
			&& symbol != ')') || quotes) && symbol != '\n'){
			if (symbol == '"')
				quotes = !quotes;
			else{
				if (ampersand || line || arrow_right){
					met_space(&first, &last, array, &str, count);
					initial_values(&count, &size, &array);
					line = ampersand = arrow_right = 0;
				}
				met_symbol(&count, &size, &array, symbol);
			}
		}
		else{
                	if (count && !ampersand && !line && !arrow_right)
				met_space(&first, &last, array, &str, count);
			else
			if (ampersand || line || arrow_right){
				if ((ampersand && symbol == '&') || 
				(line && symbol == '|')  
				|| (arrow_right && symbol == '>'))
					met_symbol(&count, &size, &array, symbol);
				met_space(&first, &last, array, &str, count);
				line = ampersand = arrow_right = 0;
				double_worked = 1;
			}
			if (symbol == '\n')
				met_enter(&first, &last, &quotes);
			initial_values(&count, &size, &array);
			if (!double_worked && ((!ampersand && symbol == '&')
				|| (!line && symbol == '|') || symbol == ';' 
				|| (!arrow_right && symbol == '>') || symbol == '<' 
				|| symbol == '(' || symbol == ')')){
				special_symbol = 1;
				if (symbol == '&')
					ampersand = !ampersand;
				if (symbol == '|')
					line = !line;
				if (symbol == '>')
					arrow_right = !arrow_right;
				met_symbol(&count, &size, &array, symbol);
				if (!(ampersand || line || arrow_right)){
					met_space(&first, &last, array, &str, count);
					initial_values(&count, &size, &array);
				}
			}
			double_worked = 0;
		}	
	}
	printf("\n");
}

int main(int argc, char **argv)
{
	if (argc == 2){
		FILE *read_from_here;
		if ((read_from_here = fopen(argv[1], "r")) == NULL){
			printf("Failed to open the file, try again\n");
			return 1;
		}
		separate(read_from_here);
		fclose(read_from_here);	
	}
	else
		separate(stdin);
	return 0;
}
