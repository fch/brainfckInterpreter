#include "stdio.h"
#include <stdlib.h>
#include <string.h>

// simple cell of tape
typedef struct Cell {

	char value;
	struct Cell *left;
	struct Cell *right;

} Cell;

// tape
typedef struct Tape {

	Cell *current;

} Tape;

// constructor for tape
// () => (Tape*)
Tape *new_tape(void) {

	return calloc(1, sizeof(Tape));

}

// constructor for cell, with neighbour, value and possible left insertion
// (in: Cell*, in: char, in: left) => (Cell*)
Cell *new_cell(Cell *neighbour, char value, int left) {

	Cell *c = calloc(1, sizeof(Cell));
	c->value = value;

	if(neighbour == NULL){
		c->left = NULL;
		c->right = NULL;
	}
	else if(left) {
		c->left = NULL;
		c->right = neighbour;
		neighbour->left = c;
	}
	else {
		c->left = neighbour;
		c->right = NULL;
		neighbour->right = c;
	}

	return c;

}



// counts chars in a given file
// (in: FILE*) => (int)
int get_file_char_count(FILE* fp){

	int file_length = 1;

	while(getc(fp) != EOF)
		file_length++;

	rewind(fp);

	return file_length;

}

// saves content of a file in a given char array
// (inout: char[], in: FILE*) => ()
void read_file(char c[], FILE *fp){

	int i = 0;
	int current;

	while((current = getc(fp)) != EOF){
		c[i] = current;
		i++;
	}

	c[i] = EOF;

	rewind(fp);

}

// interprets a char array as brainfuck sourcecode
// (in: char[]) => ()
void interpret_bf_src(char c[], int file_length){

	Tape *tape = new_tape();
	tape->current = new_cell(NULL, 0, 0);

	int i = 0;

	// test for any bf relevant char
	for(; i < file_length && c[i] != EOF; i++){

		switch(c[i]){

			case '+':
				tape->current->value++;
				break;
			case '-':
				if(tape->current->value > 0){
					tape->current->value--;
				}
				else{
					printf("Error: Negative cell value");
					exit(EXIT_FAILURE);
				}
				break;
			case '>':
				if(tape->current->right != NULL){
					tape->current = tape->current->right;
				}
				else{
					tape->current->right = new_cell(tape->current, 0, 0);
					tape->current = tape->current->right;
				}
				break;
			case '<':
				if(tape->current->left != NULL){
					tape->current = tape->current->left;
				}
				else{
					tape->current->left = new_cell(tape->current, 0, 1);
					tape->current = tape->current->left;
				}
				break;
			case '.':
				printf("%c", tape->current->value);
				break;
			case ',':
				printf("Input char: ");
				tape->current->value = getchar();
				break;
			case '[':
				if(tape->current->value == 0)
					for(; c[i] != ']'; i++)
						if(i > file_length){
							printf("Error: Missing ']'");
							exit(EXIT_FAILURE);
						}
				break;
			case ']':
				if(tape->current->value != 0)
					for(; c[i] != '['; i--)
						if(i < 0){
							printf("Error: Missing '['");
							exit(EXIT_FAILURE);
						}
				break;
			default:
				break;

		}

	}

}

int main(int argc, char *argv[]){

	//--- FILE INPUT ---
	FILE *fp;

	fp = fopen(argv[1], "r");

	if(fp == NULL){
		fprintf(stderr, "cat: cannot open %s\n", argv[1]);
	}

	int file_length = get_file_char_count(fp);
	
	char c[file_length];
	
	read_file(c, fp);
	
	fclose(fp);
	//--- END FILE INPUT ---
	
	//--- COMPILE ---
	interpret_bf_src(c, file_length);

	return 0;
}
