#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define FALSE	0
#define TRUE	1

#define FILE_NAME_SIZE	100

char input_file_name[FILE_NAME_SIZE];
char output_file_name[FILE_NAME_SIZE];

char default_input = TRUE;
char default_output = TRUE;
char auto_seed = TRUE;
char print_dim = FALSE;		//This option prints the dimensions of the matrix in the begining of the file. Change this by using 
char auto_read = FALSE;		//This option disable program's asking for inputs

int main(){
	int seed;
	int lin;
	int col;
	int max;
	FILE* fp = NULL;
	int i, j;
	
	srand(time(NULL));
	printf("Seed = ");
	scanf("%d", &seed);
	
	printf("Nro linhas = ");
	scanf("%d", &lin);
	
	printf("Nro colunas = ");
	scanf("%d", &col);
	
	printf("Valor maximo = ");
	scanf("%d", &max);
	
	fp = fopen("matriz.txt", "wt");
	if(fp == NULL){
		printf("Could not create file.\n");
		exit(0);
	}
	srand(seed);
	
	fprintf(fp, "%d %d\n", lin, col);
	
	for(i = 0; i < lin; i++){
		for(j = 0; j < col; j++){
			fprintf(fp, "%d ", (rand()%max));
		}
		fprintf(fp, "\n");
	}
	
	fclose(fp);
	return 0;
}

