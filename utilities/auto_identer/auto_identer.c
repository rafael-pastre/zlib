#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR_SIZE	50
#define LINE_SIZE	500
#define HELP_STR	"help"
#define OUT_PREFIX	"idented_"

#define IN_ERROR	"Could not open input file.\n"
#define OUT_ERROR	"Could not create output file.\n"

void clrscr();
void display_title();
void display_help_menu();
void fprinttabs(FILE* fp, int n);
char* first_digit(char* str);

int main(){
	
	char str[STR_SIZE];
	char output_file_name[STR_SIZE];
	char line[LINE_SIZE];
	FILE* input = NULL;
	FILE* output = NULL;
	int tab_num = 0;
	
	// Program initialization
	display_title();
	printf("Insert file name: ");
	scanf("%s", str);
	
	// Help menu
	if(!strcmp(str, HELP_STR)){
		display_help_menu();
		exit(0);
	}
	
	// Open input file
	input = fopen(str, "rt");
	if(input == NULL){
		printf(IN_ERROR);
		exit(0);
	}

	// Create input file
	strcpy(output_file_name, OUT_PREFIX);
	strcat(output_file_name, str);
	output = fopen(output_file_name, "wt");
	if(output == NULL){
		printf(OUT_ERROR);
		exit(0);
	}
	
	// Ident file
	while(fscanf(input, "%[^\n]", line) != EOF){
		fgetc(input);
		
		if(strchr(line, '}') != NULL){
			tab_num--;
		}
		
		fprinttabs(output, tab_num);
		fprintf(output, "%s\n", first_digit(line));
		
		if(strchr(line, '{') != NULL){
			tab_num++;
		}
		
		strcpy(line, "\0");
	}
	
	printf("File idented succesfully!\n");
	return 0;
}

void clrscr(){
    system("@cls||clear");
}

void display_title(){
	clrscr();
	printf("##########################################################\n");
	printf("#             Welcome to Zenith Auto Identer             #\n");
	printf("##########################################################\n");
}

void display_help_menu(){
	printf("\nWelcome to Zenith Auto Identer.\n");
	printf("To use it just insert the complete file name in the program.\n");
}

void fprinttabs(FILE* fp, int n){
	if(fp == NULL){
		printf(OUT_ERROR);
		exit(0);
	}
	
	while(n-- > 0)
		fprintf(fp, "\t");
}

char* first_digit(char* str){
	char* ret = str;
	while(*ret == ' ' || *ret == '\t')
		ret++;
	return ret;
}