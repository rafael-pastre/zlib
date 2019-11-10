#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FNAME_LEN 100
#define LINE_LEN 100

char* to_hex(int i);

int main()
{
    FILE* fp1;
    FILE* fp2;
	char type = 't';
	char c1, c2;
    char name1[FNAME_LEN];
    char name2[FNAME_LEN];
    char line1[LINE_LEN];
    char line2[LINE_LEN];
    char* ret1;
    char* ret2;
    int i;

	printf("Digite o tipo dos arquivos (t/b): ");
	scanf("%c", &type);
	
    printf("Digite o nome do arquivo 1: ");
    //gets(name1);
	scanf("%s", name1);
	if(type == 'b')
		fp1 = fopen(name1, "rb");
	else
		fp1 = fopen(name1, "rt");
    if(fp1 == NULL){
        printf("Nao foi possivel abrir %s.\n", name1);
        return 0;
    }
	
    printf("Digite o nome do arquivo 2: ");
	scanf("%s", name2);
    //gets(name2);
	if(type == 'b')
		fp2 = fopen(name2, "rb");
	else
		fp2 = fopen(name2, "rt");
    if(fp2 == NULL){
        printf("Nao foi possivel abrir %s.\n", name2);
        return 0;
    }

	if(type == 'b'){
		fseek(fp1, 0, SEEK_SET);
		fseek(fp2, 0, SEEK_SET);
		i = 0;
		
		while(fread(&c1, 1, 1, fp1) != 0 && fread(&c2, 1, 1, fp2) != 0 ){
			if(c1 != c2)
				printf("Diferenca no caracter %d.\n", i);
			i++;
		}
		printf("Caracteres percorridos: %d\n", i);
	}
	else{
		ret1 = fgets(line1, LINE_LEN, fp1);
		ret2 = fgets(line2, LINE_LEN, fp2);
		i = 1;

		while(ret1 != NULL && ret2 != NULL){
			if(strcmp(line1, line2))
				printf("Diferenca na linha %d.\n", i);

			i++;
			ret1 = fgets(line1, LINE_LEN, fp1);
			ret2 = fgets(line2, LINE_LEN, fp2);
		}
		printf("Linhas percorridas: %d\n", i);
	}
    return 0;
}

char* to_hex(int x){
	char* ret;
	ret = malloc(10*sizeof(char));
	
}