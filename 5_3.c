#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

struct Node
{
	char *word;
	struct Node *next;
};

void put_out(struct Node *list)
{
	while (list) {
		struct Node *tmp = list;	
        	printf("%s\n", tmp->word);
        	free(tmp->word);
		free(tmp);
        	list = list->next;
    	}
}

void delete(struct Node *list)
{
	while (list) {
		struct Node *temporary = list;
		free(temporary->word);
		free(temporary);
		list = list->next;
	}
}

char* duplicate(char *array, int *length, char symbol) /*duplicate array's length and
						copy*/
{
    int i;
	char *new_array;
	new_array = malloc(*length*2);
	for (i = 0;i < *length;i++)
		*(new_array+i) = *(array + i);
	*(new_array + i) = symbol;
	*length = (*length)*2;
	free(array);
	return new_array;
};

char* optimize(char *array, char *new_array, int length)
{
    int i;
	new_array = malloc(length + 1);
	for(i = 0;i < length;i++)
		*(new_array + i) = *(array + i);
	*(new_array + i) = 0;
	return new_array;
}

struct Node* new_word(struct Node **first, struct Node *last, char *array)
{
	struct Node *temporary;
	temporary = malloc(sizeof(struct Node));
	temporary->word = array;
	temporary->next = NULL;
	if (last) {
		last->next = temporary;
		last = last->next;
	}
	else {
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

void array_from_list(struct Node *pointer, int programs, char **comandline)
{
	for (int i = 0;i < programs;i++) {
		comandline[i] = pointer->word;
		pointer = pointer->next;
	}
	comandline[programs] = NULL;
}

void conveyor(char **comandline)
{
    int words = 0, i = 0, j = 0, k = 0, fd[2], process_id;
    while (comandline[words])
        words++;
    char **new_comandline = malloc(sizeof(char *) * words);
    while (i < words) {
        while ((i < words) && (strcmp(new_comandline[i], "|") != 0)) {
            new_comandline[k] = comandline[i];
            i++;
            k++;
        }
        /*new_comandline[k] = NULL;*/
        pipe(fd);
        process_id = fork();
        if (process_id == -1) {
            printf("Error with fork\n");
            exit(1);
        }
        if (process_id == 0) {
            if (i < (words - 1))
                dup2(fd[1], 1);
            close(fd[0]);
            close(fd[1]);
            execvp(new_comandline[0], new_comandline);
            printf("Error with execvp\n");
            exit(1);
        }
        dup2(fd[0], 0);
        close(fd[0]);
        close(fd[1]);
        for (j = 0; j < k; j++)
            new_comandline[j] = NULL;
        k = 0;
        i++;
    }
    while ((process_id = wait(NULL)) != -1);
    free(new_comandline);
    exit(0);
}

int compare_cd(char *word)
{
	int result = 0;
	int word_length = 0;
	while (word[word_length])
		word_length++;
	if (word_length == 2) {
		int if_first_element_c = (word[0] == 'c');
		int if_second_element_d = (word[1] == 'd');
		int if_word_is_cd = if_first_element_c && if_second_element_d;
		if (if_word_is_cd)
			result = 1;
	}
	return result;
}

int met_arrow(char *arrow, char **comandline, int programs)
{
    int i;
    for (i = 0; i < programs;i++) {
        if (strcmp(arrow, comandline[i]) == 0)
            return i;
    }
    return -1;
}

void process_new(char **comandline, int programs)
{
	int process_id;
	int directory;
	char error[] = "Error, failed to open directory\n";
	if (compare_cd(comandline[0])) {
		if (programs > 2)
			printf("Incorrect input, 'cd' may have 0 or 1 arguments\n");
		else {
			if (programs == 1) {
				directory = chdir(getenv("HOME"));
				if (directory == -1)
					printf("%s", error);
			}
			else {
				directory = chdir(comandline[1]);
				if (directory == -1)
					printf("%s", error);
			}
		}
	}
	else {
		int i = 0,  k = 0, words_count = 0;
        while (comandline[words_count])
            words_count++;
        char **parameters = malloc(words_count * sizeof(char *));
        int fd[2];
        process_id = fork();
        if (process_id == -1) {
                printf("Error with fork\n");
                exit(-1);
        }
        if (process_id == 0) {
            int position_input = met_arrow("<", comandline, programs);
            int conveer = met_arrow("|", comandline, programs);
            int position_output1 = met_arrow(">", comandline, programs);
            int position_output2 = met_arrow(">>", comandline, programs);
            int position_output;
            if ((position_output1 == -1) && (position_output2 == -1))
                position_output = -1;
            else
                if ((position_output1 > 0) && (position_output2 > 0)) {
                    position_output = -1;
                    printf("Incorrect input\n");
                    exit(1);
                }
                else
                    if (position_output1 > 0)
                        position_output = position_output1;
                    else
                        position_output = position_output2;
            int right_order = ((conveer > 0) && (position_input < conveer) && (((position_output > 0)
                        && (conveer < position_output)) || (position_output == -1)))
                        || ((conveer == -1) && ((position_output == -1) || ((position_output > 0)
                        && (position_input < position_output))));
            if (!right_order) {
                printf("Incorrect input\n");
                exit(1);
            }
            else {
                while (i <= words_count) {
                    if (i < words_count) {
                        if (strcmp(comandline[i], ">") == 0) {
                            int file = open(comandline[i + 1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
                            if (file == -1) {
                                printf("File error\n");
                                    exit(1);
                                }
                            i++;
                            dup2(file, 1);
                            close(file);
                        } else
                            if (strcmp(comandline[i], ">>") == 0) {
                                int file = open(comandline[i + 1], O_WRONLY |   O_APPEND      | O_CREAT, 0644);
                                if (file == -1) {
                                    printf("File error\n");
                                    exit(1);
                                }
                                dup2(file, 1);
                                close(file);
                                i++;
                            } else
                                if (strcmp(comandline[i], "<") == 0) {
                                    int file = open(comandline[i + 1], O_RDONLY, 0644);
                                    if (file == -1) {
                                        printf("File error\n");
                                        exit(1);
                                    }
                                    dup2(file, 0);
                                    close(file);
                                    i++;
                                } else
                                    parameters[k] = comandline[i];
                        i++;
                        k++;
                    } else {
                        if (parameters[k])
                            parameters[k] = NULL;
                        conveyor(parameters);
                        for (int j = 0; j < k + 1; j++) {
                            parameters[j] = NULL;
                        }
                        k = 0;
                        i++;
                    }
                }
                free(parameters);
                exit(0);
            }
        }
        wait(NULL);
        wait(NULL);
	}
}

void met_enter(struct Node *first, struct Node *last, int *flag, int programs)
{
	if (*flag)
		print_error(flag, &first);
        else
		if (first) {
			char **comandline;
			comandline = malloc(sizeof(char *)*(programs + 1));
			array_from_list(first, programs, comandline);
			process_new(comandline, programs);
			delete(first);
			free(comandline);
		}
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
        *last = new_word(first, *last, *string);
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
	int programs = 0;	
	array = malloc(size);
	char invitation[] = ">>>> : ";
	printf("%s", invitation);
	while ((symbol = getc(file)) != EOF) {
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

		if (not_string_end) {
			if (symbol_quotes)
				quotes = !quotes;
			else {
				
				if (ampersand || line || arrow_right) {
					met_space(&first, &last, array, &str, count);
					initial_values(&count, &size, &array);
					line = ampersand = arrow_right = 0;
				}
				met_symbol(&count, &size, &array, symbol);
			}
		}
		else {
			int double_special_symbol_possible = (ampersand
				|| line || arrow_right);
			int double_ampersand = (ampersand && symbol_ampersand);
			int double_line = (line && symbol_line);
			int double_arrow_right = (arrow_right && symbol_arrow_right);
                	if (count && !ampersand && !line && !arrow_right) {
				met_space(&first, &last, array, &str, count);
				programs++;
			}
			else
			if (double_special_symbol_possible) {
				if (double_ampersand || double_line  
				|| double_arrow_right)
					met_symbol(&count, &size, &array, symbol);
				met_space(&first, &last, array, &str, count);
				programs++;
				line = ampersand = arrow_right = 0;
				double_worked = 1;
			}
			if (symbol == '\n') {
				met_enter(first, last, &quotes, programs);
				first = last = NULL;
				printf("%s", invitation);
				programs = 0;
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
					programs++;
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
