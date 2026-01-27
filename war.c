#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Criação da struct
typedef struct{
char nome[30];
char cor[10];
int tropas;
}Territorio;

int main(){
//Declaração das variáveis e dos vetores;
    int i;
    Territorio territorio[5];
//Inserção de dados 
    for (i=0; i < 5; i ++){
        printf("Insira o nome do Território: ");
        scanf("%s", territorio[i].nome);
        printf("Insira a cor do Exercito: ");
        scanf("%s", territorio[i].cor);
        printf("Insira a quantidade de Tropas: ");
        scanf("%d", &territorio[i].tropas);
        printf("\n-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n");
        printf("Você inseriu o %dº território", i + 1);
        printf("\n-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n");
    }
    //Exibição das informações inseridas
    for (int i= 0; i < 5; i++) {
        printf("\n-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n");
        printf("Território %d\n", i + 1);
        printf("Nome   : %s\n", territorio[i].nome);
        printf("Cor    : %s\n", territorio[i].cor);
        printf("Tropas : %d\n", territorio[i].tropas);
        printf("\n-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*\n");
        }

return 0;
}