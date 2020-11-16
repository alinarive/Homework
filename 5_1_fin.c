#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

struct Node
{
	char *word;
	struct Node *next;
};

void put_out(struct Node *list)
{
	while (list){
		struct Node *tmp = list;	
        	printf("%s\n", tmp->word);
        	free(tmp->word);
		free(tmp);
        	list = list->next;
    	}
}

void delete(struct Node *list)
{
	while (list){
		struct Node *temporary = list;
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

struct Node* new_word(char *array, struct Node **first, struct Node *last)
{
	struct Node *temporary;
	temporary = malloc(sizeof(struct Node));
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

void print_error(int *flag, struct Node **pointer)
{
	char error[] = "Error, please enter correct number of quotes\n";
	printf("%s", error);
        *flag = 0;
        delete(*pointer);
}


void for_next_itterarion(struct Node **first, struct Node **last)
{
	char invitation[] = "Invitation: ";
	*first = *last = NULL;
	printf("%s", invitation);
}

void met_enter(struct Node *first, struct Node *last, int *flag, int programs)
{
	if (*flag)
		print_error(flag, &first);
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

void met_space(struct Node **first, struct Node **last, char *array, 
		char **string, int count)
{
	*string = optimize(array, *string, count);
        *last = new_word(*string, first, *last);
}
	 
void separate(FILE *file)
{
	int symbol;
	struct Node *first = NULL, *last = NULL;
	int count = 0;				/*counter for word's length*/
	int size = 8;				/*start length*/
	char *array, *str;			/*array of letters*/
	/*flags for special symbols, not about current one*/
	int quotes = 0, ampersand = 0, line = 0, arrow_right = 0, special_symbol = 0,
		double_worked = 0;
	array = malloc(size);
	for_next_itteration(&first, &last);
	while ((symbol = getc(file)) != EOF){
		/*to define if current is a special symbol*/
		int symbol_space = (symbol == ' ');
		int symbol_carriage = (symbol == '\r');
		int symbol_tabulation = (symbol == '\t');
		int symbol_ampersand = (symbol == '&');
		int symbol_line = (symbol == '|');
		int symbol_semicolon = (symbol == ';');
		int symbol_arrow_right = (symbol == '>');
		int symbol_arrow_left = (symbol == '<');
		int symbol_bracket_left = (symbol == '(');
		int symbol_bracket_right = (symbol == ')');
		int symbol_enter = (symbol == '\n');
		int symbol_quotes = (symbol == '"');
		int not_special_symbol_or_quotes = ((!symbol_space 
			&& !symbol_carriage && !symbol_tabulation 
			&& !symbol_ampersand && !symbol_line 
			&& !symbol_semicolon && !symbol_arrow_right 
			&& !symbol_arrow_left && !symbol_bracket_left 
			&& !symbol_bracket_right) || quotes);
		int not_string_end = not_special_symbol_or_quotes && !symbol_enter;

		if (not_string_end){
			if (symbol_quotes)
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
			int double_special_symbol_possible = (ampersand
				|| line || arrow_right);
			int double_ampersand = (ampersand && symbol_ampersand);
			int double_line = (line && symbol_line);
			int double_arrow_right = (arrow_right && symbol_arrow_right);
                	if (count && !ampersand && !line && !arrow_right)
				met_space(&first, &last, array, &str, count);
			else
			if (double_special_symbol_possible){
				if (double_ampersand || double_line  
				|| double_arrow_right)
					met_symbol(&count, &size, &array, symbol);
				met_space(&first, &last, array, &str, count);
				line = ampersand = arrow_right = 0;
				double_worked = 1;
			}
			if (symbol == '\n'){
				met_enter(first, last, &quotes, programs);
			}
			initial_values(&count, &size, &array);
			int first_ampersand = (!ampersand && symbol_ampersand);
			int first_line = (!line && symbol_line);
			int first_arrow_right = (!arrow_right && symbol_arrow_right);
			int first_special_symbol = (!double_worked &&
				(first_ampersand || first_line
				|| first_arrow_right || symbol_semicolon
				|| symbol_arrow_left || symbol_bracket_left
				|| symbol_bracket_right));
			if (first_special_symbol){				
				special_symbol = 1;
				if (symbol_ampersand)
					ampersand = !ampersand;
				if (symbol_line)
					line = !line;
				if (symbol_arrow_right)
					arrow_right = !arrow_right;
				met_symbol(&count, &size, &array, symbol);
				if (!double_special_symbol_possible){
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
